/*
	GWEN
	Copyright (c) 2010 Facepunch Studios
	See license in Gwen.h
*/


#include "../gwen.h"
#include "../gwen_skin.h"
#include "gwen_labelclickable.h"

using namespace Gwen;
using namespace Gwen::Controls;

GWEN_CONTROL_CONSTRUCTOR( LabelClickable )
{
	SetIsToggle( false );

	SetCursor( Gwen::CursorType::Finger );
	SetAlignment( Gwen::Pos::Left | Gwen::Pos::CenterV );
}

void LabelClickable::Render( Skin::Base* /*skin*/ )
{
	//skin->DrawButton( this, IsDepressed(), IsToggle() && GetToggleState() );
}