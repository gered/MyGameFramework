#include "../debug.h"
#include "../log.h"

#include "../common.h"
#include "gwen_spritebatchrenderer.h"
#include <gwen.h>
#include <gwen_font.h>
#include <gwen_structures.h>
#include <gwen_textobject.h>
#include <gwen_texture.h>
#include "../content/contentmanager.h"
#include "../content/spritefontparam.h"
#include "../graphics/color.h"
#include "../graphics/graphicsdevice.h"
#include "../graphics/image.h"
#include "../graphics/spritebatch.h"
#include "../graphics/spritefont.h"
#include "../graphics/texture.h"
#include <stl/string.h>

#define LOGCAT_GWENUI "GWENUI"

namespace Gwen
{
	namespace Renderer
	{
		SpriteBatchRenderer::SpriteBatchRenderer(ContentManager *contentManager, GraphicsDevice *graphicsDevice)
		{
			m_spriteBatch = NULL;
			m_contentManager = contentManager;
			m_graphicsDevice = graphicsDevice;
			m_alpha = COLOR_ALPHA_OPAQUE;
		}

		SpriteBatchRenderer::~SpriteBatchRenderer()
		{
		}

		void SpriteBatchRenderer::PreRender(SpriteBatch *spriteBatch)
		{
			m_spriteBatch = spriteBatch;
		}

		void SpriteBatchRenderer::PostRender()
		{
			m_spriteBatch = NULL;
		}

		void SpriteBatchRenderer::Begin()
		{
			ASSERT(m_spriteBatch != NULL);
			Gwen::Renderer::Base::Begin();
		}

		void SpriteBatchRenderer::End()
		{
			Gwen::Renderer::Base::End();
		}

		void SpriteBatchRenderer::SetDrawColor(Gwen::Color color)
		{
			m_color = ::Color(color.r, color.g, color.b, color.a);
		}

		void SpriteBatchRenderer::DrawFilledRect(Gwen::Rect rect)
		{
			Translate(rect);

			::Color renderColor = AdjustColorForAlpha(m_color);
			::Texture *colorTexture = m_graphicsDevice->GetSolidColorTexture(renderColor);

			m_spriteBatch->Render(colorTexture, rect.x, rect.y, rect.w, rect.h);
		}

		void SpriteBatchRenderer::StartClip()
		{
			ASSERT(m_spriteBatch != NULL);
			Gwen::Rect rect = ClipRegion();

			int16_t left = (int16_t)((float)rect.x * Scale());
			int16_t top = (int16_t)((float)rect.y * Scale());
			int16_t right = (int16_t)((float)(rect.x + rect.w) * Scale());
			int16_t bottom = (int16_t)((float)(rect.y + rect.h) * Scale());

			m_spriteBatch->SetClipRegion(left, top, right, bottom);
		}

		void SpriteBatchRenderer::EndClip()
		{
			ASSERT(m_spriteBatch != NULL);
			m_spriteBatch->ClearClipRegion();
		}

		void SpriteBatchRenderer::LoadTexture(Gwen::Texture *pTexture)
		{
			const stl::string textureFilename = pTexture->name.Get();
			LOG_INFO(LOGCAT_GWENUI, "Gwen::Renderer::SpriteBatchRenderer loading texture \"%s\"\n", textureFilename.c_str());
			::Texture *texture = m_contentManager->Get< ::Texture>(textureFilename);
			ASSERT(texture != NULL);

			pTexture->data = texture;
			pTexture->width = texture->GetWidth();
			pTexture->height = texture->GetHeight();
		}

		void SpriteBatchRenderer::FreeTexture(Gwen::Texture *pTexture)
		{
			const stl::string textureFilename = pTexture->name.Get();
			::Texture *texture = (::Texture*)pTexture->data;
			ASSERT(texture != NULL);

			if (textureFilename.size() == 0)
				LOG_INFO(LOGCAT_GWENUI, "Gwen::Renderer::SpriteBatchRenderer not freeing texture ID %d. Has empty name property, was not loaded by us.\n", texture->GetTextureName());
			else
			{
				LOG_INFO(LOGCAT_GWENUI, "Gwen::Renderer::SpriteBatchRenderer freeing texture \"%s\"\n", textureFilename.c_str());
				m_contentManager->Free< ::Texture>(texture);
			}
		}

		void SpriteBatchRenderer::DrawTexturedRect(Gwen::Texture *pTexture, Gwen::Rect pTargetRect, float u1, float v1, float u2, float v2)
		{
			Translate(pTargetRect);
			::Texture *texture = (::Texture*)pTexture->data;
			ASSERT(texture != NULL);

			::Color renderColor = AdjustColorForAlpha(::Color(1.0f, 1.0f, 1.0f));

			m_spriteBatch->Render(texture, pTargetRect.x, pTargetRect.y, pTargetRect.w, pTargetRect.h, u1, v1, u2, v2, renderColor);
		}

		Gwen::Color SpriteBatchRenderer::PixelColour(Gwen::Texture *pTexture, unsigned int x, unsigned int y, const Gwen::Color &col_default)
		{
			// HACK: sloppy, figure out a better way
			//       although this is possibly not that terrible since using
			//       ContentManager to load an image will cause the
			//       ContentLoader<Image> object to cache the image for subsequent
			//       access... the only real problem is that the image will be
			//       loaded twice per SpriteBatchRenderer instance (LoadTexture + this)
			//       and that right now we don't free the image
			//       Should probably have an std::list of Image objects to be freed
			//       in the destructor
			const stl::string textureFilename = pTexture->name.Get();
			Image *image = m_contentManager->Get<Image>(textureFilename);
			ASSERT(image != NULL);

			::Color pixelColor = image->GetColor(x, y);

			Gwen::Color result;
			pixelColor.ToInts(&result.r, &result.g, &result.b, &result.a);
			return result;
		}

		void SpriteBatchRenderer::LoadFont(Gwen::Font *pFont)
		{
			LOG_INFO(LOGCAT_GWENUI, "Gwen::Renderer::SpriteBatchRenderer loading font \"%s\"\n", pFont->facename.c_str());
			SpriteFontParam fontParam((uint8_t)pFont->size);
			SpriteFont *font = m_contentManager->Get<SpriteFont>(pFont->facename, fontParam);
			ASSERT(font != NULL);

			pFont->data = font;
		}

		void SpriteBatchRenderer::FreeFont(Gwen::Font *pFont)
		{
			// HACK: apparently this can get called when some set of UI elements
			//       is set up and then later freed, but no fonts were ever
			//       rendered vi GWEN. In this case, pFont->data will be NULL...
			if (pFont->data == NULL)
				return;

			LOG_INFO(LOGCAT_GWENUI, "Gwen::Renderer::SpriteBatchRenderer freeing font \"%s\"\n", pFont->facename.c_str());
			SpriteFont *font = (SpriteFont*)pFont->data;
			ASSERT(font != NULL);

			m_contentManager->Free<SpriteFont>(font);
		}

		void SpriteBatchRenderer::RenderText(Gwen::Font *pFont, Gwen::Point pos, const Gwen::String &text)
		{
			Translate(pos.x, pos.y);
			SpriteFont *font = (SpriteFont*)pFont->data;
			ASSERT(font != NULL);

			::Color renderColor = AdjustColorForAlpha(m_color);

			m_spriteBatch->Render(font, pos.x, pos.y, renderColor, Scale(), text.c_str());
		}

		Gwen::Point SpriteBatchRenderer::MeasureText(Gwen::Font *pFont, const Gwen::String &text)
		{
			SpriteFont *font = (SpriteFont*)pFont->data;

			// HACK: is this supposed to work this way? seems that MeasureText 
			//       can (and will) get called from Gwen's classes before a call
			//       to LoadFont is made...
			if (font == NULL)
			{
				LoadFont(pFont);
				font = (SpriteFont*)pFont->data;
			}

			ASSERT(font != NULL);

			uint16_t width = 0;
			uint16_t height = 0;
			font->MeasureString(&width, &height, text.c_str());

			Gwen::Point result(width, height);
			return result;
		}

		::Color SpriteBatchRenderer::AdjustColorForAlpha(const ::Color &color) const
		{
			::Color result = color;
			result.a *= m_alpha;
			return result;
		}
	}
}

