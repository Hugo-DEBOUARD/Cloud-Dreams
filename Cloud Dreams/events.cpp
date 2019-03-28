// events.cpp

#include <iostream>

#include <irrlicht.h>

#include "events.h"
//#include "gui_ids.h"

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;
namespace ig = irr::gui;

/**************************************************************************\
 * EventReceiver::EventReceiver                                           *
\**************************************************************************/
EventReceiver::EventReceiver()
  : node(nullptr)
  {}

/*------------------------------------------------------------------------*\
 * EventReceiver::keyboard_handler                                        *
\*------------------------------------------------------------------------*/
bool EventReceiver::keyboard_handler(const SEvent &event)
{
 
    switch (event.KeyInput.Key)
      {
        case KEY_ESCAPE:
          exit(0);
	  
	case KEY_UP: // Saut
	  jump = event.KeyInput.PressedDown;
          break;
 
        case KEY_RIGHT: // Avance
	  avance = event.KeyInput.PressedDown;

	  break;
	  
        case KEY_LEFT: // Recule
	  recule = event.KeyInput.PressedDown;

          break;
      
      	case KEY_KEY_P:
	  play = true;
	  
	  break;
	  
	case KEY_KEY_F:
	  suicide = true;
	  
	  break;
	  
	case KEY_KEY_R:
	  
	  if(event.KeyInput.PressedDown && release_r)
	  {
	    release_r = false;
	    reset = true;
	  }
	  if (!event.KeyInput.PressedDown)
	    release_r = true;
	  
	  break;
	  
	  	  
	default:;  
	
      }
	 
  

  return false;
}



void EventReceiver::move(bool &is_he_running, bool &is_he_standing,bool &is_he_jumping,
                                    float &motion_direction)
{
    ic::vector3df position = node->getPosition();
    ic::vector3df rotation = node->getRotation();
    
      if(avance)
      {
	position.X += 2.0f;
	if(motion_direction<0)
	{
	rotation.Y = 0;
	motion_direction = 1;
	}
	
      }
    
      if(recule)
      {
        position.X += -2.0f;
	if(motion_direction>0)
	{
	 rotation.Y = 180;
	 motion_direction = -1;
	}
      }


    if(jump)
    {
      position.Y += 5.0f;
    }    

    if(jump) // si la touche de saut est activée
    { 
      is_he_standing = false;             // le personnage n'est pas debout
      is_he_running = false;
      if(!is_he_jumping)                  // s'il n'est pas déja en train de sauter
      {
	is_he_jumping = node->setMD2Animation(irr::scene::EMAT_JUMP);  // il doit sauter maintenant
      }
    }
    else if(avance || recule) // si une touche de déplacement est activée
    { 
      is_he_standing = false;             // le personnage n'est pas debout
      is_he_jumping = false;
      if(!is_he_running)                  // s'il n'est pas déja en train de courir
      {  
	is_he_running = node->setMD2Animation(irr::scene::EMAT_RUN);  // il doit courir maintenant
      }
    
    }
    else                                  // si la touche de déplacement n'est pas activée
    {
      is_he_running = false;              // le personnage ne cours pas	
      is_he_jumping = false;
      
      if(!is_he_standing)                  // s'il n'est pas déja debout
      {  
	is_he_standing = node->setMD2Animation(irr::scene::EMAT_STAND);  // il doit être debout maintenant
      }
    }

    node->setPosition(position);
    node->setRotation(rotation);

}






/*------------------------------------------------------------------------*\
 * EventReceiver::gui_handler                                             *
\*------------------------------------------------------------------------*/

/*
bool EventReceiver::gui_handler(const SEvent &event)
{
  if (!node) return false;
  switch(event.GUIEvent.EventType)
  {
    // Gestion des menus de la barre de menu
    case ig::EGET_MENU_ITEM_SELECTED:
      {
        ig::IGUIContextMenu *menu = (ig::IGUIContextMenu*)event.GUIEvent.Caller;
        s32 item = menu->getSelectedItem();
        s32 id = menu->getItemCommandId(item);
        u32 debug_info = node->isDebugDataVisible();

        switch(id)
        {
          case MENU_NEW_GAME:
            node->setPosition(ic::vector3df(0,0,0));
	    node->setRotation(ic::vector3df(0,0,0));
	    u32 menu_size = menu->getItemCount();
	    
	    for(u32 k=0;k<menu_size;k++)
	    {
	      ig::IGUIContextMenu *submenu = menu->getSelectedItem();
	    }
	    
	    break;
	    
	    
          case MENU_QUIT:
            exit(0);

          case MENU_BOUNDING_BOX:
            menu->setItemChecked(item, !menu->isItemChecked(item));
            node->setDebugDataVisible(debug_info ^ is::EDS_BBOX);
            break;

          case MENU_NORMALS:
            menu->setItemChecked(item, !menu->isItemChecked(item));
            node->setDebugDataVisible(debug_info ^ is::EDS_NORMALS);
            break;

          case MENU_TRIANGLES:
            menu->setItemChecked(item, !menu->isItemChecked(item));
            node->setDebugDataVisible(debug_info ^ is::EDS_MESH_WIRE_OVERLAY);
            break;

          case MENU_TRANSPARENCY:
            menu->setItemChecked(item, !menu->isItemChecked(item));
            node->setDebugDataVisible(debug_info ^ is::EDS_HALF_TRANSPARENCY);
            break;

          case MENU_ABOUT:
            gui->addMessageBox(L"Boite About", L"Texte présentant ce super jeu\nd'un intérêt incroyable");
            break;
        }
      }
      break;
      // gestion des boites d'édition de texte
    case ig::EGET_EDITBOX_CHANGED:
      {
        s32 id = event.GUIEvent.Caller->getID();
        if (id == WINDOW_VALUE)
        {
          ic::stringc s = event.GUIEvent.Caller->getText();
          std::cout << "editbox changed:" << s.c_str() << std::endl;
        }
      }
      break;
      // gestion des boutons
    case ig::EGET_BUTTON_CLICKED:
      {
        s32 id = event.GUIEvent.Caller->getID();
        if (id == WINDOW_BUTTON)
          std::cout << "Button clicked\n";
      }
      break;
      // gestion des cases à cocher
    case ig::EGET_CHECKBOX_CHANGED:
      {
        s32 id = event.GUIEvent.Caller->getID();
        if (id == WINDOW_CHECK_BOX)
        {
          std::cout << "Check box clicked: ";
          bool checked = ((ig::IGUICheckBox*)event.GUIEvent.Caller)->isChecked();
          if (!checked) std::cout << "un";
          std::cout << "checked\n";
        }
      }
      break;
      // gestion des combo-box
    case ig::EGET_COMBO_BOX_CHANGED:
      {
        s32 id = event.GUIEvent.Caller->getID();
        if (id == WINDOW_COMBO_BOX)
        {
          ig::IGUIComboBox *cbox = (ig::IGUIComboBox*)event.GUIEvent.Caller;
          s32 item = cbox->getSelected();
          u32 elem_id = cbox->getItemData(item);
          std::cout << "Combo box changed: item " << item << ", id " << elem_id << std::endl;
        }
      }
      break;
      // Gestion des listes
    case ig::EGET_LISTBOX_CHANGED:
      {
        s32 id = event.GUIEvent.Caller->getID();
        if (id == WINDOW_LIST_BOX)
        {
          ig::IGUIListBox *lbox = (ig::IGUIListBox*)event.GUIEvent.Caller;
          s32 item = lbox->getSelected();
          std::cout << "List box changed: item " << item << std::endl;
        }
      }
      break;
      // Gestion des barres de défilement
    case ig::EGET_SCROLL_BAR_CHANGED:
      {
        s32 id = event.GUIEvent.Caller->getID();
        if (id == WINDOW_SCROLLBAR)
        {
          ig::IGUIScrollBar *scroll = (ig::IGUIScrollBar*)event.GUIEvent.Caller;
          s32 value = scroll->getPos();
          std::cout << "Scrollbar moved: " << value << std::endl;
        }
      }
      break;
      // Gestion des spinbox
    case ig::EGET_SPINBOX_CHANGED:
      {
        s32 id = event.GUIEvent.Caller->getID();
        if (id == WINDOW_SPIN_BOX)
        {
          ig::IGUISpinBox *spin = (ig::IGUISpinBox*)event.GUIEvent.Caller;
          f32 value = spin->getValue();
          std::cout << "Spin Box changed: " << value << std::endl;
        }
      }
      break;
    default:;
  }
  return false;
}

*/


/**************************************************************************\
 * EventReceiver::OnEvent                                                 *
\**************************************************************************/
bool EventReceiver::OnEvent(const SEvent &event)
{
  if (!node) return false;
  switch (event.EventType)
  {
    case EET_KEY_INPUT_EVENT:
      return keyboard_handler(event);
//    case EET_GUI_EVENT:
//      return gui_handler(event);
    default:;
  }

  return false;
}

/**************************************************************************\
 * EventReceiver::set_node                                                *
\**************************************************************************/
void EventReceiver::set_node(irr::scene::IAnimatedMeshSceneNode *n)
{
  node = n;
}

/**************************************************************************\
 * EventReceiver::set_gui                                                 *
\**************************************************************************/
/*
void EventReceiver::set_gui(irr::gui::IGUIEnvironment *g)
{
  gui = g;
}
*/

void EventReceiver::set_actions(bool &j, bool &a, bool &r)
{
  jump = j;
  avance = a;
  recule = r;
}

