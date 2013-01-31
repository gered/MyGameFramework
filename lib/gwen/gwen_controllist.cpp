/*
	GWEN
	Copyright (c) 2012 Facepunch Studios
	See license in Gwen.h
*/


#include "gwen.h"
#include "gwen_controllist.h"

using namespace Gwen;
using namespace Gwen::Controls;

void ControlList::Enable()
{
	for ( List::const_iterator it = list.begin(); it != list.end(); ++it )
	{
		(*it)->SetDisabled( false );
	}
}

void ControlList::Disable()
{
	for ( List::const_iterator it = list.begin(); it != list.end(); ++it )
	{
		(*it)->SetDisabled( true );
	}
}

void ControlList::Show()
{
	for ( List::const_iterator it = list.begin(); it != list.end(); ++it )
	{
		(*it)->Show();
	}
}

void ControlList::Hide()
{
	for ( List::const_iterator it = list.begin(); it != list.end(); ++it )
	{
		(*it)->Hide();
	}
}

void ControlList::MoveBy( const Gwen::Point& point )
{
	for ( List::const_iterator it = list.begin(); it != list.end(); ++it )
	{
		(*it)->MoveBy( point.x, point.y );
	}
}

void ControlList::DoAction()
{
	for ( List::const_iterator it = list.begin(); it != list.end(); ++it )
	{
		(*it)->DoAction();
	}
}

void ControlList::SetActionInternal( Gwen::Event::Handler* pObject, void (Gwen::Event::Handler::*f)( Gwen::Event::Info ), const Gwen::Event::Packet& packet )
{
	for ( List::const_iterator it = list.begin(); it != list.end(); ++it )
	{
		(*it)->SetAction( pObject, f, packet );
	}
}