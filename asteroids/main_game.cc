#include "renderer.h"
#include "sdl2_renderer.h"
#include "game.h"
#include "physics.h"
#include "game_controller.h"
#include "sdl2_game_controller.h"
 
#ifdef _WIN32
#include <windows.h>
int main(void);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
    return main();
}
#endif

// sets up the model, view, and controller objects
// main itself is a controller containing the game main loop
int main(void) {
  Timer timer;
  Game game{};
  SDL2GameController controller = SDL2GameController{game};
  SDL2Renderer renderer = SDL2Renderer{game, "Asteroids"};
  
  renderer.init();
  do {
    timer.reset();
    renderer.render();
    controller.do_user_interactions();
    if ( ! controller.exit_game() ) {
      controller.do_game_events();
      timer.tick_and_delay( controller.get_tick_time() );
    }
  } while (! controller.exit_game() );
  renderer.exit();

  return 0;
}
