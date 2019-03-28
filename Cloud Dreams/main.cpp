#include <irrlicht.h>

#include <iostream>
#include <string>
#include <vector>
#include "events.h"

#include "math.h"

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;
namespace ig = irr::gui;

  /**************************************************************************************************/
  // Fonction de création du sol en nuage ---- paramètres : - smgr - scale du modèle - position
  /**************************************************************************************************/

is::IMeshSceneNode *create_ground(is::ISceneManager *smgr,
				  ic::vector3df scale, ic::vector3df position)
{
    is::IAnimatedMesh *mesh_ground = smgr->getMesh("data/BigCloudStep/BigCloudStep.obj");
    is::IMeshSceneNode *node_ground = smgr->addOctreeSceneNode(mesh_ground->getMesh(0),nullptr,-4);
    node_ground->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    node_ground->setScale(scale);  
    node_ground->setPosition(position);
    
    // Création du triangle selector
    scene::ITriangleSelector *selector_ground;
    selector_ground = smgr->createOctreeTriangleSelector(node_ground->getMesh(), node_ground);
    node_ground->setTriangleSelector(selector_ground);
  
  return node_ground;
}  
  
  /**************************************************************************************************/
  // Fonction de création de plateformes ---- paramètres : - smgr - scale du modèle - position - numéro de la plateforme
  /**************************************************************************************************/
  
is::IAnimatedMeshSceneNode *create_platform(is::ISceneManager *smgr,
					    ic::vector3df scale, ic::vector3df position,int platform_nbr)
{
    is::IAnimatedMesh *mesh_platform = smgr->getMesh("data/BigCloudStep/BigCloudStep.obj");
    is::IAnimatedMeshSceneNode *node_platform = smgr->addAnimatedMeshSceneNode(mesh_platform,nullptr,platform_nbr);
    node_platform->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    node_platform->setScale(scale);
    node_platform->setMaterialTexture(0,(smgr->getVideoDriver())->getTexture("data/BigCloudStep/BigCSS00.png"));
    node_platform->setPosition(position);
    
  return node_platform;
}

  /**************************************************************************************************/
  // Fonction de création des murs ---- paramètres : - smgr - numéro du mur
  /**************************************************************************************************/
  

is::IMeshSceneNode *create_wall(is::ISceneManager *smgr,int wall_number)
{
   //Création d'un plan pour le mur différente méthode !
  is::IAnimatedMesh *mesh_wall = smgr->addHillPlaneMesh("wall", ic::dimension2d<f32>(34.56, 23.04), ic::dimension2d<u32>(100,100));

  is::IMeshSceneNode *node_wall = smgr->addOctreeSceneNode(mesh_wall->getMesh(0),nullptr,-wall_number);  
  
  node_wall->setMaterialFlag(irr::video::EMF_LIGHTING, false);  
  
  // Création du triangle selector
    is::ITriangleSelector *selector_wall;
    selector_wall = smgr->createOctreeTriangleSelector(node_wall->getMesh(), node_wall);
    node_wall->setTriangleSelector(selector_wall);
    
    ic::vector3df rotation = node_wall->getRotation();
  
  switch (wall_number)
      {
        case 1:       // fond de la scène
	  node_wall->setRotation(ic::vector3df(-90,0,0));
	  node_wall->setPosition(core::vector3df(0, 0, 1000));
	  break;
 
	  //mur de limite gauche
        case 2: 
	  node_wall->setScale(ic::vector3df(1,1,0.1));
	  node_wall->setRotation(ic::vector3df(rotation.X,rotation.Y,90));
	  node_wall->setPosition(core::vector3df(200, 0, 0.34));
	  break;
	  //mur de limite droit
        case 3: 
	  node_wall->setScale(ic::vector3df(1,1,0.1));
	  node_wall->setRotation(ic::vector3df(rotation.X,rotation.Y,-90));
	  node_wall->setPosition(core::vector3df(-200, 0, 0.34));
          break;
	  
	default:;  
      }
      
      return node_wall;
}

  /**************************************************************************************************/
  // Fonction de reset quand on presse R ---> pas au point pour une raison inconnue la personnage 
  // semble affecté par la gravité plus que demandé et les plateformes volantes n'ont plus de collision
  /**************************************************************************************************/
  

void initialise(int &score, irr::ITimer *timer, is::IAnimatedMeshSceneNode *pers, is::IAnimatedMeshSceneNode *chrono, is::IAnimatedMeshSceneNode *ange, scene::ICameraSceneNode *camera, bool &is_he_standing,
  float &motion_direction, bool &mort, bool &review, bool &parti_chrono, bool &parti_ange, bool &win, EventReceiver &receiver, is::ISceneNodeAnimatorCollisionResponse *anim_ground,
  is::IMetaTriangleSelector *selector_ground, is::ISceneManager *smgr, is::IAnimatedMesh *mesh_pers, iv::IVideoDriver  *driver, is::IMeshSceneNode *node_wall_side1, 
  is::IMeshSceneNode *node_wall_side2, is::IAnimatedMeshSceneNode *node_platform_r, is::IMeshSceneNode *node_ground)
{
  delete pers;
  mort = false;
  review = false;
  parti_ange = false;
  parti_chrono = false;
  win = false;
  motion_direction = 1;
  timer->setTime(0);
  camera->setPosition(ic::vector3df(0, 10, -2800));
  camera->setTarget(ic::vector3df(0, 0, 0));
  mesh_pers = smgr->getMesh("data/tris.md2");
  pers = smgr->addAnimatedMeshSceneNode(mesh_pers, nullptr, -5);
  receiver.set_node(pers);
  pers->setMaterialFlag(irr::video::EMF_LIGHTING, false);
  pers->setMD2Animation(irr::scene::EMAT_STAND);
  pers->setScale(ic::vector3df(0.25,0.25,0.25));
  pers->setPosition(ic::vector3df(-190,0,0));
  pers->setMaterialTexture(0,driver->getTexture("data/blue_texture.pcx"));
  ange->setVisible(true);
  chrono->setVisible(true);
  chrono->setMaterialTexture(0,driver->getTexture("data/faerie2.bmp"));
  score = 1001;
  receiver.suicide = false;
  receiver.reset = false;
  receiver.play = false;
  selector_ground = smgr->createMetaTriangleSelector();
  selector_ground->addTriangleSelector(node_wall_side1->getTriangleSelector());
  selector_ground->addTriangleSelector(node_wall_side2->getTriangleSelector());
  selector_ground->addTriangleSelector(node_platform_r->getTriangleSelector());
  selector_ground->addTriangleSelector(node_ground->getTriangleSelector());
  anim_ground = smgr->createCollisionResponseAnimator(selector_ground, 
                                               pers,  // Le noeud que l'on veut gérer
                                               ic::vector3df(5, 5, 5), // "rayons" du perso
                                               ic::vector3df(0, -10, 0),  // gravité
                                               ic::vector3df(0, 5, 0));  // décalage du centre
  
  
  pers->addAnimator(anim_ground);

}

  /**************************************************************************************************/
  // Fonction vérifiant si il y collision entre deux nodes
  /**************************************************************************************************/
  

bool caTouche(is::IAnimatedMeshSceneNode *node_pers, is::IAnimatedMeshSceneNode *node_chrono)
{
  ic::vector3df pos_pers = node_pers->getPosition();
  const ic::aabbox3d<f32>& box_pers = node_pers->getTransformedBoundingBox();
//   node_pers->setDebugDataVisible(is::EDS_BBOX);
  ic::vector3df rayon_pers = (box_pers.MaxEdge - box_pers.getCenter());
  
  ic::vector3df pos_chrono = node_chrono->getPosition();
  const ic::aabbox3d<f32>& box_chrono= node_chrono->getTransformedBoundingBox();
//   node_chrono->setDebugDataVisible(is::EDS_BBOX);
  ic::vector3df rayon_chrono = (box_chrono.MaxEdge - box_chrono.getCenter());
  
  ic::vector3df ecart = pos_pers - pos_chrono;
  ecart.set(abs(ecart.X), abs(ecart.Y), abs(ecart.Z));
  
  bool collision = true;
  collision = collision && (ecart.X < rayon_chrono.X + rayon_pers.X);
  collision = collision && (ecart.Y < rayon_chrono.Y + rayon_pers.Y);
  collision = collision && (ecart.Z < rayon_chrono.Z + rayon_pers.Z);
  
  return collision;
}



int main()
{
  
  /**************************************************************************************************/
  // Initialisation de la scène (fenêtre -> driver -> scene manager)
  /**************************************************************************************************/
  
  // Le gestionnaire d'événements
  EventReceiver receiver;
  
  // Création de la fenêtre et du système de rendu.
  IrrlichtDevice *device = createDevice(iv::EDT_OPENGL,
                                        ic::dimension2d<u32>(640, 480),
                                        16, false, false, false, &receiver);

  iv::IVideoDriver  *driver = device->getVideoDriver();
  is::ISceneManager *smgr = device->getSceneManager();
  ig::IGUIEnvironment *gui = device->getGUIEnvironment();
  irr::ITimer *timer = device->getTimer();

  /**************************************************************************************************/
  // Création du personnage dans la scène (mesh -> node , texture, scale et position dans la scène
  /**************************************************************************************************/

  is::IAnimatedMesh *mesh_pers = smgr->getMesh("data/tris.md2");
  is::IAnimatedMeshSceneNode *node_pers = smgr->addAnimatedMeshSceneNode(mesh_pers,nullptr,-5);
  receiver.set_node(node_pers);
  node_pers->setMaterialFlag(irr::video::EMF_LIGHTING, false);
  node_pers->setMD2Animation(irr::scene::EMAT_STAND);
  node_pers->setMaterialTexture(0,driver->getTexture("data/blue_texture.pcx"));
  ic::vector3df position_perso_init = node_pers->getPosition();
  node_pers->setScale(ic::vector3df(0.25,0.25,0.25));
  node_pers->setPosition(ic::vector3df(-180,0,0));
  
  /**************************************************************************************************/
  // Création du fond et des murs de limite de coté et texture du fond
  /**************************************************************************************************/
   
  is::IMeshSceneNode *node_wall_back =  create_wall(smgr,1);
  node_wall_back->setMaterialTexture(0,driver->getTexture("data/cloud.jpg"));
  is::IMeshSceneNode *node_wall_side1 =  create_wall(smgr,2);
  is::IMeshSceneNode *node_wall_side2 =  create_wall(smgr,3);
  
   /**************************************************************************************************/
   // Création du MetaTriangle qui va permettre de gérer la collision entre le personnage et le décor (base + mur + plateformes)
   /**************************************************************************************************/

  is::IMetaTriangleSelector *decor_triangle_selector = smgr->createMetaTriangleSelector();

  
  // ajout des murs limites de coté au MetaTriangle
  decor_triangle_selector->addTriangleSelector(node_wall_side1->getTriangleSelector());
  decor_triangle_selector->addTriangleSelector(node_wall_side2->getTriangleSelector());
  
  /**************************************************************************************************/
  // Création de quelques plateformes dans le jeu
  //
  // le rand() permet de fixer en hauteur (Y) aléatoirement la plateforme suivant la précédente (avec une différence de hauteur minimale)
  // mais aussi la différence de distance entre les 2 plateformes (en X avec décal_x)
  //
  
  /**************************************************************************************************/
  int Ny = 5;
  float offset = 30.0f;
  ic::vector3df former_platform_pos = ic::vector3df(0, 0, 0);
  bool add = true;
  
  for(int ky=0;ky<Ny;ky++)
  {
    float decal_x = rand()%100;
    int random_y = rand()%51;
    ic::vector3df current_platform_pos;
    if(add==true)
      current_platform_pos = ic::vector3df(former_platform_pos.X+decal_x,former_platform_pos.Y+offset+random_y,0);
    else
      current_platform_pos = ic::vector3df(former_platform_pos.X-decal_x,former_platform_pos.Y+offset+random_y,0);

    is::IAnimatedMeshSceneNode *node_platform = create_platform(smgr,ic::vector3df(0.01,0.005,0.01),current_platform_pos,ky);
    
    // Création du triangle selector à ajouter au MetaTriangle pour la collision
 
    is::ITriangleSelector *selector_pers;
    selector_pers = smgr->createOctreeTriangleSelector(node_platform->getMesh(), node_platform);
    node_platform->setTriangleSelector(selector_pers);
    
    decor_triangle_selector->addTriangleSelector(selector_pers);
    former_platform_pos = current_platform_pos;
    add = !add;
  }
  
    /**************************************************************************************************/
    // Création de la plateforme du bas à une position fixe
    //
   /**************************************************************************************************/

    is::IAnimatedMeshSceneNode *node_platform_r = create_platform(smgr,ic::vector3df(0.01,0.005,0.01),ic::vector3df(0,10,0),1);
    node_platform_r->setMaterialTexture(0,driver->getTexture("data/fire.bmp"));
    node_platform_r->setMaterialType(iv::EMT_TRANSPARENT_ALPHA_CHANNEL);

    // Création du triangle selector à ajouter au MetaTrianglen decor_triangle_selector pour la collision
    //
    is::ITriangleSelector *selector_pers;
    selector_pers = smgr->createOctreeTriangleSelector(node_platform_r->getMesh(), node_platform_r);
    node_platform_r->setTriangleSelector(selector_pers);
    
    decor_triangle_selector->addTriangleSelector(node_platform_r->getTriangleSelector());
  
  /**************************************************************************************************/
  // Création du sol
  /**************************************************************************************************/

  is::IMeshSceneNode *node_ground = create_ground(smgr,ic::vector3df(1.0,0.5,0.1),core::vector3df(180, -6, 0));
  node_ground->setMaterialTexture(0,driver->getTexture("data/BigCloudStep/BigCSS00.png") ); 
  decor_triangle_selector->addTriangleSelector(node_ground->getTriangleSelector());
  

  
  /**************************************************************************************************/
  // Initialisation des paramètres de position du personnage au receveur d'évènements
  /**************************************************************************************************/

  bool jump = false;
  bool avance = false;
  bool recule = false;
  
  receiver.set_actions(jump,avance,recule);
  
  bool is_he_standing = true;
  bool is_he_running = false;
  bool is_he_jumping = false;
  
  // initialise la direction de déplacement (vecteur d'orientation) selon X
  float motion_direction = 1; 

  /**************************************************************************************************/
  // Création de la caméra fixée sur le personnage
  /**************************************************************************************************/

  ic::vector3df position_cam = ic::vector3df(0, 10, -60);
  ic::vector3df orientation_cam = position_perso_init - position_cam;
  ic::vector3df position_cam_intro = ic::vector3df(0, 10, -2800);  

  
   
  scene::ICameraSceneNode* camera = 
  smgr->addCameraSceneNode(nullptr,
                           position_cam_intro,         // position
                           orientation_cam,      // orientation
                           -1,         		 // Identifiant
                           true);      	         // camera active
			     
  /**************************************************************************************************/
  // Création de l'animation de collision utilisant le MetaTriangle (qui possède tous les triangle_selector) à gérer avec le personnage
  /**************************************************************************************************/
  
  
  is::ISceneNodeAnimatorCollisionResponse *anim_decor;
  anim_decor = smgr->createCollisionResponseAnimator(decor_triangle_selector, 
                                               node_pers,                   // Le noeud que l'on veut gérer
                                               ic::vector3df(5,5,5),      // "rayons" du perso
                                               ic::vector3df(0, -10, 0),    // gravité
                                               ic::vector3df(0, 5, 0));     // décalage du centre
  
  
  node_pers->addAnimator(anim_decor);
  /**************************************************************************************************/
  //Création de la fée qui représente l'objet bonus faisant gagner du temps
  /**************************************************************************************************/

  is::IAnimatedMesh *chrono = smgr->getMesh("data/faerie.md2");
  is::IAnimatedMeshSceneNode *node_chrono = smgr->addAnimatedMeshSceneNode(chrono);
  node_chrono->setMaterialFlag(irr::video::EMF_LIGHTING, false);
  node_chrono->setPosition(ic::vector3df(100,225,0));  
  node_chrono->setScale(ic::vector3df(0.1,0.1,0.1));
  node_chrono->setMaterialTexture(0,driver->getTexture("data/faerie2.bmp")); 
  
  /**************************************************************************************************/
  //Création de l'ange qui représente la victoire
  /**************************************************************************************************/
  
  is::IAnimatedMesh *ange = smgr->getMesh("data/ange/Angel.obj");
  is::IAnimatedMeshSceneNode *node_ange = smgr->addAnimatedMeshSceneNode(ange);
  node_ange->setMaterialFlag(irr::video::EMF_LIGHTING, false);
  node_ange->setPosition(ic::vector3df(63,346,0));  
  node_ange->setScale(ic::vector3df(1,1,1));
  node_ange->setRotation(ic::vector3df(0,180,0));
 
  
  /**************************************************************************************************/
  //Création de l'écran titre dynamique !
  /**************************************************************************************************/
  
  is::IMesh *intro = smgr->addHillPlaneMesh("intro", ic::dimension2d<f32>(51.2, 51.2), ic::dimension2d<u32>(100,100));
  is::IMeshSceneNode *intro_node = smgr->addMeshSceneNode(intro);
  intro_node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
  intro_node->setPosition(ic::vector3df(0,0,-300));
  intro_node->setRotation(ic::vector3df(-90,0,0));
  

  
  /**************************************************************************************************/
  // Création du score (appel dans le while)
  /**************************************************************************************************/
  
  // Chargement des textures pour les chiffres
  iv::ITexture *digits[10];
  digits[0] = driver->getTexture("data/0.png");
  digits[1] = driver->getTexture("data/1.png");
  digits[2] = driver->getTexture("data/2.png");
  digits[3] = driver->getTexture("data/3.png");
  digits[4] = driver->getTexture("data/4.png");
  digits[5] = driver->getTexture("data/5.png");
  digits[6] = driver->getTexture("data/6.png");
  digits[7] = driver->getTexture("data/7.png");
  digits[8] = driver->getTexture("data/8.png");
  digits[9] = driver->getTexture("data/9.png");

  // Création des places pour les chiffres
  ig::IGUIImage *score_10000 = gui->addImage(ic::rect<s32>(10,10,  50,50)); score_10000->setScaleImage(true);
  ig::IGUIImage *score_1000  = gui->addImage(ic::rect<s32>(50,10,  90,50)); score_1000->setScaleImage(true);
  ig::IGUIImage *score_100   = gui->addImage(ic::rect<s32>(90,10,  130,50)); score_100->setScaleImage(true);
  ig::IGUIImage *score_10    = gui->addImage(ic::rect<s32>(130,10, 170,50)); score_10->setScaleImage(true);
  ig::IGUIImage *score_1     = gui->addImage(ic::rect<s32>(170,10, 210,50)); score_1->setScaleImage(true);
  
  /**************************************************************************************************/
  // Chargement des textures pour la page d'intro
  /**************************************************************************************************/
  
  iv::ITexture *ecran_intro[5];
  ecran_intro[0] = driver->getTexture("data/intro1.png");
  ecran_intro[1] = driver->getTexture("data/intro2.png");
  ecran_intro[2] = driver->getTexture("data/intro3.png");
  ecran_intro[3] = driver->getTexture("data/intro4.png");
  ecran_intro[4] = driver->getTexture("data/intro5.png");
  
  // Initialisation des variables nécessaires pour le while
  int score = 1001;
  bool mort = false, review = false, parti_chrono = false, parti_ange = false, win = false;
  
  
  while(device->run())
  {
    
    driver->beginScene(true, true, iv::SColor(250,150,200,255));
    ic::vector3df position_perso = node_pers->getPosition();

    // Si le perso n'est pas encore mort et qu'on n'a pas appuyé sur P
    if (mort == false && receiver.play == false) 
    {
	intro_node->setMaterialTexture(0,ecran_intro[timer->getTime()/500 % 5]);
    }
    
    
    // Si le perso n'est pas mort, qu'on a appuyé sur P et qu'on a appuyé sur F après avoir appuyé sur P
    if (mort == false && receiver.play == true && receiver.suicide == true) 
    {
      	camera->setPosition(ic::vector3df(0, 10, -2800));
	camera->setTarget(ic::vector3df(0, 0, 0));
	intro_node->setMaterialTexture(0,driver->getTexture("data/die_fun.png"));
 	timer->setTime(0);
	mort = true;
	review = true;
	receiver.play = false;
    }
    
    
    //Si tu appuies sur P et que tu n'es pas mort (review ne s'applique qu'après la mort donc critère très pertinent)
    if(receiver.play == true && review == false)
    {
	camera->setPosition(position_perso - orientation_cam);
	camera->setTarget(position_perso);
	receiver.move(is_he_running,is_he_standing,is_he_jumping, motion_direction);
    }
    
    // Sécurité pour pas commencer le jeu déjà mort
    if (receiver.play == false && receiver.suicide == true)
      receiver.suicide = false;

    // Si tu appuies sur la touche R touche de reset --> fonction pas au point
    if (receiver.reset == true)
      initialise(score, timer, node_pers, node_chrono, node_ange, camera, is_he_standing, motion_direction,
		     mort, review, parti_chrono, parti_ange, win, receiver, anim_decor, decor_triangle_selector, smgr, mesh_pers, driver,node_wall_side1, 
		    node_wall_side2, node_platform_r, node_ground);
      
      
    if(mort == true && review == false)
    {
	camera->setPosition(ic::vector3df(0, 10, -2800));
	camera->setTarget(ic::vector3df(0, 0, 0));
	intro_node->setMaterialTexture(0,driver->getTexture("data/die.png"));
	timer->setTime(0);
	review = true;
    }
      

    if(review == true && timer->getTime() >= 5000)
      
      	intro_node->setMaterialTexture(0,driver->getTexture("data/review.png"));
    
    if(receiver.play == true)
    {
      if(score !=0)
      {
	// Calcul du score :
      score--;
      // Mise à jour du score :
      score_10000->setImage(digits[(score / 10000) % 10]);
      score_1000->setImage(digits[(score / 1000) % 10]);
      score_100->setImage(digits[(score / 100) % 10]);
      score_10->setImage(digits[(score / 10) % 10]);
      score_1->setImage(digits[(score / 1) % 10]);
      }
      else
      {
	mort = true;
	receiver.play = false;
      }

    }
    
    score_10000->setVisible(receiver.play && !mort);
    score_1000->setVisible(receiver.play && !mort);
    score_100->setVisible(receiver.play && !mort);
    score_10->setVisible(receiver.play && !mort);
    score_1->setVisible(receiver.play && !mort);
    
    if(caTouche(node_pers, node_chrono) == true && parti_chrono == false)
    {
      node_chrono->setVisible(false);
      score += 1000;
      parti_chrono = true;
    }
    
    if(caTouche(node_pers, node_ange) == true && parti_ange == false)
    {
      node_ange->setVisible(false);
      win = true;
      parti_ange = true;
    }
    
    if (win == true)
    {
        camera->setPosition(ic::vector3df(0, 10, -2800));
	camera->setTarget(ic::vector3df(0, 0, 0));
	intro_node->setMaterialTexture(0,driver->getTexture("data/win.png"));
    }
    
    smgr->drawAll();
    gui->drawAll();
    // 
    driver->endScene();
  }
  device->drop();

  return 0;
}

