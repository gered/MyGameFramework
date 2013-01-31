/*
	GWEN
	Copyright (c) 2010 Facepunch Studios
	See license in Gwen.h
*/

#pragma once
#ifndef GWEN_CONTROLS_RESIZER_H
#define GWEN_CONTROLS_RESIZER_H

#include "gwen_base.h"
#include "../gwen.h"
#include "../gwen_skin.h"
#include "gwen_dragger.h"


namespace Gwen 
{
	namespace ControlsInternal
	{
		class GWEN_EXPORT Resizer : public Dragger
		{
			public:

				GWEN_CONTROL( Resizer, Dragger );

				virtual void OnMouseMoved( int x, int y, int deltaX, int deltaY );
				virtual void SetResizeDir( int dir );

				Event::Caller	onResize;

			protected:

				int		m_iResizeDir;
				
		};
	}
}
#endif
