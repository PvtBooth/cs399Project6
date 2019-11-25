 /******************************************************************************/
/*!
	\file   Graphics.c
	\author Henry Brobeck, Ryan Booth
	\date   1/18/2017
	\par    Project: BlastDroids
	\par    Copyright © 2017 DigiPen (USA) Corporation.
	\brief
 */
/******************************************************************************/

#include "Graphics.h"
#include "Vector2.h"

#include "Entity.h"
#include "Texture.h"
#include "RenderLayer.h"
#include "Sprite.h"

/*------------------------------------------------------------------------------
// Private Consts:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Structures:
//----------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
// Public Variables:
//----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
// Private Variables:
//----------------------------------------------------------------------------*/





static AEGfxVertexList *unitMesh = NULL;
static TransformComponent *unitTransform = NULL;
static Component *unitComponent = NULL;
static Entity *unitEntity;
static RECT window;
static RECT desktop;
static WindowMode window_mode;

static AESysInitInfo sysInitInfo;

static int windowWidth = 1280; //Width of the window
static int windowHeight = 800; //Height of the window

static bool g_initialized = false;

/*------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------------------------------*/




static AEGfxVertexList *RectMesh(float width, float height, u32 color);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);



/*------------------------------------------------------------------------------
// Public Functions:
//----------------------------------------------------------------------------*/

/*!
 \brief Start the Graphics module
 */
void Graphics_Init(HINSTANCE instanceH, HINSTANCE prevInstanceH, LPSTR command_line, int show)
{
    UNREFERENCED_PARAMETER(command_line);
    UNREFERENCED_PARAMETER(prevInstanceH);

  Graphics_SetWindowMode(WINDOWED);
  sysInitInfo.mCreateWindow = 1;
  sysInitInfo.mWindowHandle = NULL;
  sysInitInfo.mAppInstance = instanceH;
  sysInitInfo.mShow = show;
  sysInitInfo.mCreateConsole = 1;
  sysInitInfo.mMaxFrameRate = 60;
  sysInitInfo.mClassStyle = CS_HREDRAW | CS_VREDRAW;
  sysInitInfo.mpWinCallBack = WindowProc;
  sysInitInfo.mHandleWindowMessages = 1;

  AESysInit(&sysInitInfo);
  Graphics_ResetWindow();
  Graphics_SetWindowPosition(WINDOWED);
  
  //AEGfxSetBackgroundColor(1.0f, 1.0f, 1.0f);
  AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
  AEGfxSetBlendMode(AE_GFX_BM_BLEND);

  unitMesh = RectMesh(1, 1, 0xFFFFFFFF);
  unitComponent = TransformComponent_Create();
  unitTransform = unitComponent->data;
  unitEntity = Entity_Create("RectEntity");
  Entity_AttachComponent(unitEntity, unitComponent);

  //Stores the window resolution
  Graphics_SetWindowResolution();

  //Window Title Name
  SetWindowText(AESysGetWindowHandle(), "TeamA001");

  g_initialized = true;
}

/*!
 \brief 
 \param	dt Change in time (in seconds) since last game loop.
 */
void Graphics_Update(float dt)
{
    UNREFERENCED_PARAMETER(dt);
}

/*!
 \brief Shut down the Graphics module
 */
void Graphics_Exit(void)
{
  Entity_DestroyOrphan(unitEntity);

  MeshGroup_DestroyAll();
  Sprite_DestroyAll();
  
  Texture_DestroyAll();
  AESysExit();
}

void Graphics_Render()
{
  //THIS FUNCTION IS DEPRECATED
//  for(int i = 0; i < RENDER_LAYER_STANDARD_MAX; i++)
//  {
//    RenderObserver *iterator = renderHeads[i];
//    while(iterator)
//    {
//      iterator->render(iterator->component);
//      iterator = iterator->next;
//    }
//    
//  }
}


void Graphics_RegisterComponentRender(void(* render)(Component* self), Component* component, RENDER_LAYER layer)
{
  //THIS FUNCTION IS DEPRECATED
    UNREFERENCED_PARAMETER(render);
    UNREFERENCED_PARAMETER(component);
    UNREFERENCED_PARAMETER(layer);
}

void Graphics_RemoveComponentRender(Component* self, int layer)
{
    UNREFERENCED_PARAMETER(self);
    UNREFERENCED_PARAMETER(layer);
  //THIS FUNCTION IS DEPRECATED
}


RECT Graphics_GetDesktopResolution()
{
    RECT desktop_res;

    // Get a handle to the desktop window
    const HWND hDesktop = GetDesktopWindow();
    // Get the size of screen to the variable desktop
    GetWindowRect(hDesktop, &desktop_res);

    return desktop_res;
}

void Graphics_SetWindowResolution()
{
    // Get a handle to the desktop window
    const HWND hWindow = AESysGetWindowHandle();
    // Get the size of screen to the variable desktop
    GetWindowRect(hWindow, &window);

}

RECT Graphics_GetWindowResolution()
{
    RECT window_h;

    const HWND hWindow = AESysGetWindowHandle();

    GetWindowRect(hWindow, &window_h);

    return window_h;
}




void Graphics_DrawRectCamera(Vector2 pos, Vector2 size, float rotation, float r, float g, float b, float a)
{
  AEGfxSetRenderMode(AE_GFX_RM_COLOR);
  TransformComponent_SetRotation(unitTransform, rotation);
  TransformComponent_SetPosition(unitTransform, pos);
  TransformComponent_SetScale(unitTransform, size);


  Matrix3x3 *matrix = TransformComponent_GetTransformMatrix(unitTransform);

  AEGfxTextureSet(NULL, 0, 0);
  // Drawing the mesh (list of triangles)
  AEGfxSetTintColor(r, g, b, a);
  AEGfxSetTransform((float(*)[3])matrix);
  AEGfxMeshDraw(unitMesh, AE_GFX_MDM_TRIANGLES);
  AEGfxSetTintColor(1, 1, 1, 1);
}


void Graphics_DrawRect(Vector2 pos, Vector2 size, float rotation, float r, float g, float b, float a)
{
  AEGfxSetRenderMode(AE_GFX_RM_COLOR);
  //AEGfxSetWorldOriginMode(AE_WOM_TOP_LEFT);
  // Set poisition for object 1
  //AEGfxSetPosition(obj1X, obj1Y);
  TransformComponent_SetRotation(unitTransform, rotation);
  TransformComponent_SetPosition(unitTransform, pos);
  TransformComponent_SetScale(unitTransform, size);
  
  
  Matrix3x3 *matrix = TransformComponent_GetCameraTransformMatrix(unitTransform);

  
  // No texture for object 1
  AEGfxTextureSet(NULL, 0, 0);
  // Drawing the mesh (list of triangles)
  AEGfxSetTintColor(r, g, b, a);
  AEGfxSetTransform((float (*)[3])matrix);
  AEGfxMeshDraw(unitMesh, AE_GFX_MDM_TRIANGLES);
  AEGfxSetTintColor(1, 1, 1, 1);
}

AEGfxVertexList* Graphics_GetUnitMesh()
{
  return unitMesh;
}

 HINSTANCE getHInstance()
 {
  if(g_initialized)
   return sysInitInfo.mAppInstance;

  return NULL;
 }

 void Graphics_SetWindowMode(WindowMode select)
{
    //Get the desktop resolution
    desktop = Graphics_GetDesktopResolution();

    int windowstyle = WS_OVERLAPPEDWINDOW;

    if (select == WINDOWED)
    {
        windowWidth = 1280;
        windowHeight = 800;
        SetWindowLongPtr(AESysGetWindowHandle(), GWL_STYLE, windowstyle | WS_VISIBLE);
    }
  
    //Sets the window to windowed borderless fullscreen
    if (select == FULLSCREEN)
    {
        windowWidth = desktop.right;
        windowHeight = desktop.bottom;

        windowstyle = WS_POPUP;

        SetWindowLongPtr(AESysGetWindowHandle(), GWL_STYLE,
            WS_SYSMENU | windowstyle | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE);
    }

    sysInitInfo.mWinWidth = windowWidth;
    sysInitInfo.mWinHeight = windowHeight;
    sysInitInfo.mWindowStyle = windowstyle;

    window_mode = select;
}

WindowMode Graphics_GetWindowMode()
{
    return window_mode;
}

void Graphics_ResetWindow()
{
    AESysReset();
}

void Graphics_SetWindowPosition(WindowMode select)
{
    if (select == FULLSCREEN)
    {
        //Sets the window to top left corner 0,0
        SetWindowPos(AESysGetWindowHandle(), HWND_TOP, 0, 0, windowWidth, windowHeight, SWP_SHOWWINDOW);
    }
    else
        SetWindowPos(AESysGetWindowHandle(), HWND_TOP, 0 + ((desktop.right - windowWidth) / 2), 0 + ((desktop.bottom - windowHeight) / 2), windowWidth, windowHeight, SWP_SHOWWINDOW);
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/



static AEGfxVertexList *RectMesh(float width, float height, u32 color)
{

    AEGfxMeshStart();

    AEGfxTriAdd(
        -width / 2, -height / 2, color, 0.0f, 1.0f,
        width / 2, -height / 2, color, 1, 1,
        -width / 2, height / 2, color, 0.0f, 0.0f);



    AEGfxTriAdd(
        width / 2, -height / 2, color, 1, 1,
        width / 2, height / 2, color, 1, 0.0f,
        -width / 2, height / 2, color, 0.0f, 0.0f);


    AEGfxVertexList *mesh = AEGfxMeshEnd();
    assert(mesh);
    return mesh;
}




LRESULT CALLBACK WindowProc(
    HWND hwnd,        // handle to window
    UINT uMsg,        // message identifier
    WPARAM wParam,    // first message parameter
    LPARAM lParam)    // second message parameter
{
    //hwnd = AESysGetWindowHandle();
    switch (uMsg)
    {
    case WM_EXITSIZEMOVE:
        //Window gets resized
        Graphics_SetWindowResolution();
        //Debug
        //printf("Window changed size to: %ld, %ld", window.right - window.left, window.bottom - window.top);
        return 0;
        

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}