#include "../framework/debug.h"

#include "textureanimator.h"
#include "textureatlasanimationsequence.h"
#include "../framework/content/contentmanager.h"
#include "../framework/graphics/graphicsdevice.h"
#include "../framework/graphics/image.h"
#include "../framework/graphics/texture.h"
#include "../framework/graphics/textureatlas.h"
#include <stl/string.h>

TextureAnimator::TextureAnimator(ContentManager *contentManager, GraphicsDevice *graphicsDevice)
{
	STACK_TRACE;
	m_contentManager = contentManager;
	m_graphicsDevice = graphicsDevice;
}

TextureAnimator::~TextureAnimator()
{
	STACK_TRACE;
	ResetAll();
}

void TextureAnimator::ResetAll()
{
	for (TextureAtlasAnimations::iterator i = m_textureAtlasAnimations.begin(); i != m_textureAtlasAnimations.end(); ++i)
	{
		TextureAtlasAnimationSequence &sequence = *i;
		if (sequence.originalAnimatingTile != NULL)
		{
			RestoreTextureWithOriginalTile(sequence);
			SAFE_DELETE(sequence.originalAnimatingTile);
		}
		if (sequence.frames != NULL)
		{
			for (uint32_t i = 0; i < sequence.GetNumFrames(); ++i)
			{
				Image *frame = sequence.frames[i];
				SAFE_DELETE(frame);
			}
		}
	}

	m_textureAtlasAnimations.clear();
}

void TextureAnimator::AddTileSequence(TextureAtlas *atlas, uint32_t tileToBeAnimated, uint32_t start, uint32_t stop, float delay, BOOL loop)
{
	STACK_TRACE;
	AddTileSequence("", atlas, tileToBeAnimated, start, stop, delay, loop);
}

void TextureAnimator::AddTileSequence(const stl::string &name, TextureAtlas *atlas, uint32_t tileToBeAnimated, uint32_t start, uint32_t stop, float delay, BOOL loop)
{
	STACK_TRACE;
	ASSERT(atlas != NULL);
	ASSERT(tileToBeAnimated < atlas->GetNumTextures());
	ASSERT(start < atlas->GetNumTextures());
	ASSERT(stop < atlas->GetNumTextures());
	ASSERT(start < stop);
	ASSERT((tileToBeAnimated >= start && tileToBeAnimated <= stop) == FALSE);

	TextureAtlasAnimationSequence *existingSequence = FindTileSequenceByName(name);
	ASSERT(existingSequence == NULL);
	if (existingSequence != NULL)
		return;

	TextureAtlasAnimationSequence sequence;
	sequence.atlas = atlas;
	sequence.animatingIndex = tileToBeAnimated;
	sequence.start = start;
	sequence.stop = stop;
	sequence.delay = delay;
	sequence.isAnimating = TRUE;
	sequence.loop = loop;
	sequence.frames = new Image*[sequence.GetNumFrames()];
	sequence.name = name;

	sequence.current = sequence.start;
	sequence.currentFrameTime = 0.0f;

	// since we can't read a texture back from OpenGL after we've uploaded it
	// (??? or can we somehow .. ?), we need to load the image again so that
	// we can copy out the image data for tiles "start" to "stop"
	stl::string textureFilename = m_contentManager->GetNameOf<Texture>((Texture*)atlas->GetTexture());
	Image *textureImage = m_contentManager->Get<Image>(textureFilename);

	// first, copy the original tile image that's at the "tileToBeAnimated" spot
	// in the atlas texture so we can restore it back again if necessary
	const TextureAtlasTile &originalTile = atlas->GetTile(sequence.animatingIndex);
	sequence.originalAnimatingTile = new Image();
	sequence.originalAnimatingTile->Create(textureImage, originalTile.dimensions.left, originalTile.dimensions.top, originalTile.dimensions.GetWidth(), originalTile.dimensions.GetHeight());

	// copy each frame ("start" to "stop") from the source texture image
	for (uint32_t i = 0; i < sequence.GetNumFrames(); ++i)
	{
		const TextureAtlasTile &tile = atlas->GetTile(i + sequence.start);
		sequence.frames[i] = new Image();
		sequence.frames[i]->Create(textureImage, tile.dimensions.left, tile.dimensions.top, tile.dimensions.GetWidth(), tile.dimensions.GetHeight());
	}

	m_textureAtlasAnimations.push_back(sequence);
}

void TextureAnimator::ResetTileSequence(const stl::string &name)
{
	STACK_TRACE;
	ASSERT(name.length() != 0);

	TextureAtlasAnimationSequence *existingSequence = FindTileSequenceByName(name);
	ASSERT(existingSequence != NULL);

	existingSequence->isAnimating = TRUE;
	existingSequence->current = existingSequence->start;
	existingSequence->currentFrameTime = 0.0f;

	UpdateTextureWithCurrentTileFrame(*existingSequence);
}

void TextureAnimator::StopTileSequence(const stl::string &name, BOOL restoreOriginalTile)
{
	STACK_TRACE;
	ASSERT(name.length() != 0);

	TextureAtlasAnimationSequence *existingSequence = FindTileSequenceByName(name);
	ASSERT(existingSequence != NULL);

	existingSequence->isAnimating = FALSE;
	existingSequence->current = existingSequence->stop;
	existingSequence->currentFrameTime = 0.0f;

	if (restoreOriginalTile)
		RestoreTextureWithOriginalTile(*existingSequence);
	else
		UpdateTextureWithCurrentTileFrame(*existingSequence);
}

void TextureAnimator::EnableTileSequence(const stl::string &name, BOOL enable)
{
	ASSERT(name.length() != 0);

	TextureAtlasAnimationSequence *existingSequence = FindTileSequenceByName(name);
	ASSERT(existingSequence != NULL);

	existingSequence->isAnimating = enable;
	existingSequence->currentFrameTime = 0.0f;

	UpdateTextureWithCurrentTileFrame(*existingSequence);
}


void TextureAnimator::OnUpdate(float delta)
{
	STACK_TRACE;
	for (TextureAtlasAnimations::iterator i = m_textureAtlasAnimations.begin(); i != m_textureAtlasAnimations.end(); ++i)
	{
		TextureAtlasAnimationSequence &sequence = *i;

		if (!sequence.IsAnimationFinished() && sequence.isAnimating)
		{
			sequence.currentFrameTime += delta;

			if (sequence.currentFrameTime >= sequence.delay)
			{
				// move to the next frame
				sequence.currentFrameTime = 0.0f;

				++sequence.current;
				if (sequence.current > sequence.stop)
					sequence.current = sequence.start;

				UpdateTextureWithCurrentTileFrame(sequence);
			}
		}
	}
}

void TextureAnimator::OnNewContext()
{
	STACK_TRACE;
	for (TextureAtlasAnimations::iterator i = m_textureAtlasAnimations.begin(); i != m_textureAtlasAnimations.end(); ++i)
	{
		TextureAtlasAnimationSequence &sequence = *i;
		UpdateTextureWithCurrentTileFrame(sequence);
	}
}

TextureAtlasAnimationSequence* TextureAnimator::FindTileSequenceByName(const stl::string &name)
{
	STACK_TRACE;
	if (name.length() == 0)
		return NULL;

	for (TextureAtlasAnimations::iterator i = m_textureAtlasAnimations.begin(); i != m_textureAtlasAnimations.end(); ++i)
	{
		const TextureAtlasAnimationSequence &sequence = *i;
		if (sequence.name == name)
			return &(*i);
	}

	return NULL;
}

void TextureAnimator::UpdateTextureWithCurrentTileFrame(TextureAtlasAnimationSequence &sequence)
{
	STACK_TRACE;
	uint32_t frameIndex = sequence.current - sequence.start;
	ASSERT(frameIndex < sequence.GetNumFrames());
	Image *frameImage = sequence.frames[frameIndex];
	const TextureAtlasTile &tile = sequence.atlas->GetTile(sequence.animatingIndex);

	sequence.atlas->GetTexture()->Update(frameImage, tile.dimensions.left, tile.dimensions.top);
}

void TextureAnimator::RestoreTextureWithOriginalTile(TextureAtlasAnimationSequence &sequence)
{
	STACK_TRACE;
	const TextureAtlasTile &tile = sequence.atlas->GetTile(sequence.animatingIndex);
	sequence.atlas->GetTexture()->Update(sequence.originalAnimatingTile, tile.dimensions.left, tile.dimensions.top);
}

