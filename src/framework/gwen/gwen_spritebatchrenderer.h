#ifndef __FRAMEWORK_GWEN_GWEN_SPRITEBATCHRENDERER_H_INCLUDED__
#define __FRAMEWORK_GWEN_GWEN_SPRITEBATCHRENDERER_H_INCLUDED__

#include <gwen_baserender.h>
#include "../graphics/color.h"

class ContentManager;
class GraphicsDevice;
class SpriteBatch;

namespace Gwen
{
	struct Color;
	struct Font;
	struct Point;
	struct Rect;
	struct Texture;

	namespace Renderer
	{
		class SpriteBatchRenderer : public Gwen::Renderer::Base
		{
		public:
			SpriteBatchRenderer(ContentManager *contentManager, GraphicsDevice *graphicsDevice);
			virtual ~SpriteBatchRenderer();

			void PreRender(SpriteBatch *spriteBatch);
			void PostRender();

			void Begin();
			void End();

			void SetDrawColor(Gwen::Color color);

			void DrawFilledRect(Gwen::Rect rect);

			void StartClip();
			void EndClip();

			void LoadTexture(Gwen::Texture *pTexture);
			void FreeTexture(Gwen::Texture *pTexture);
			void DrawTexturedRect(Gwen::Texture *pTexture, Gwen::Rect pTargetRect, float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f);
			Gwen::Color PixelColour(Gwen::Texture *pTexture, unsigned int x, unsigned int y, const Gwen::Color &col_default = Gwen::Color(255, 255, 255, 255));

			void LoadFont(Gwen::Font *pFont);
			void FreeFont(Gwen::Font *pFont);
			void RenderText(Gwen::Font *pFont, Gwen::Point pos, const Gwen::String &text);
			Gwen::Point MeasureText(Gwen::Font *pFont, const Gwen::String &text);

			float GetAlpha() const                         { return m_alpha; }
			void SetAlpha(float alpha)                     { m_alpha = alpha; }

		private:
			::Color AdjustColorForAlpha(const ::Color &color) const;

			ContentManager *m_contentManager;
			GraphicsDevice *m_graphicsDevice;
			SpriteBatch *m_spriteBatch;
			::Color m_color;
			float m_alpha;
		};
	}
}

#endif

