#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <stack>
#include <queue>
#include <vector>
using namespace std;

const int winValue = 128;

const SDL_Color gameBackgroundColor{247, 233, 220, 255};

const int margins = 15;

const SDL_Color boardBackgroundColor{187, 173, 160, 255};

int board[4][4];

struct BoardCell {
  int value;
  bool wasModified;
} tmpBoardCell;

queue<BoardCell> tmpBoardCellQueue;

SDL_Window *window;

SDL_Renderer *renderer;

SDL_Event *event;

SDL_Surface *s, *sGameOver, *sGameWon;

SDL_Texture *tex2, *tex4, *tex8, *tex16;
SDL_Texture *tex32, *tex64, *tex128, *tex256;
SDL_Texture *tex512, *tex1024, *tex2048;
SDL_Texture *texGameOver, *texGameWon;

enum Direction {
  up = 0,
  down,
  left,
  right
};

struct BoardCellPosition {
  int x, y;
} tmpBoardCellPosition;

vector<BoardCellPosition> available;

void upLogic() {
  while(!tmpBoardCellQueue.empty()) {
    tmpBoardCellQueue.pop();
  }
  for(int j = 0; j < 4; j++) {
    for(int i = 0; i < 4; i++) {
      if(board[i][j] == 0) {
        continue;
      }
      if(!tmpBoardCellQueue.empty() && board[i][j] == tmpBoardCellQueue.back().value && !tmpBoardCellQueue.back().wasModified) {
        tmpBoardCellQueue.back().value = tmpBoardCellQueue.back().value << 1;
        tmpBoardCellQueue.back().wasModified = true;
      } else {
        tmpBoardCell.value = board[i][j];
        tmpBoardCell.wasModified = false;
        tmpBoardCellQueue.push(tmpBoardCell);
      }
    }
    for(int i = 0; i < 4; i++) {
      if(!tmpBoardCellQueue.empty()) {
        board[i][j] = tmpBoardCellQueue.front().value;
        tmpBoardCellQueue.pop();
      } else {
        board[i][j] = 0;
      }
    }
  }
}

void downLogic() {
  while(!tmpBoardCellQueue.empty()) {
    tmpBoardCellQueue.pop();
  }
  for(int j = 0; j < 4; j++) {
    for(int i = 3; i >= 0; i--) {
      if(board[i][j] == 0) {
        continue;
      }
      if(!tmpBoardCellQueue.empty() && board[i][j] == tmpBoardCellQueue.back().value && !tmpBoardCellQueue.back().wasModified) {
        tmpBoardCellQueue.back().value = tmpBoardCellQueue.back().value << 1;
        tmpBoardCellQueue.back().wasModified = true;
      } else {
        tmpBoardCell.value = board[i][j];
        tmpBoardCell.wasModified = false;
        tmpBoardCellQueue.push(tmpBoardCell);
      }
    }
    for(int i = 3; i >= 0; i--) {
      if(!tmpBoardCellQueue.empty()) {
        board[i][j] = tmpBoardCellQueue.front().value;
        tmpBoardCellQueue.pop();
      } else {
        board[i][j] = 0;
      }
    }
  }
}

void leftLogic() {
  while(!tmpBoardCellQueue.empty()) {
    tmpBoardCellQueue.pop();
  }
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      if(board[i][j] == 0) {
        continue;
      }
      if(!tmpBoardCellQueue.empty() && board[i][j] == tmpBoardCellQueue.back().value && !tmpBoardCellQueue.back().wasModified) {
        tmpBoardCellQueue.back().value = tmpBoardCellQueue.back().value << 1;
        tmpBoardCellQueue.back().wasModified = true;
      } else {
        tmpBoardCell.value = board[i][j];
        tmpBoardCell.wasModified = false;
        tmpBoardCellQueue.push(tmpBoardCell);
      }
    }
    for(int j = 0; j < 4; j++) {
      if(!tmpBoardCellQueue.empty()) {
        board[i][j] = tmpBoardCellQueue.front().value;
        tmpBoardCellQueue.pop();
      } else {
        board[i][j] = 0;
      }
    }
  }
}

void rightLogic() {
  while(!tmpBoardCellQueue.empty()) {
    tmpBoardCellQueue.pop();
  }
  for(int i = 0; i < 4; i++) {
    for(int j = 3; j >= 0; j--) {
      if(board[i][j] == 0) {
        continue;
      }
      if(!tmpBoardCellQueue.empty() && board[i][j] == tmpBoardCellQueue.back().value && !tmpBoardCellQueue.back().wasModified) {
        tmpBoardCellQueue.back().value = tmpBoardCellQueue.back().value << 1;
        tmpBoardCellQueue.back().wasModified = true;
      } else {
        tmpBoardCell.value = board[i][j];
        tmpBoardCell.wasModified = false;
        tmpBoardCellQueue.push(tmpBoardCell);
      }
    }
    for(int j = 3; j >= 0; j--) {
      if(!tmpBoardCellQueue.empty()) {
        board[i][j] = tmpBoardCellQueue.front().value;
        tmpBoardCellQueue.pop();
      } else {
        board[i][j] = 0;
      }
    }
  }
}

void gameOver() {
  int winW, winH;
  puts("GAME OVER");
  SDL_GetWindowSize(window, &winW, &winH);
  SDL_Rect *goPos = new SDL_Rect;

  goPos->w = sGameOver->w;
  goPos->h = sGameOver->h;
  goPos->x = (winW - goPos->w) >> 1;
  goPos->y = (winH - goPos->h) >> 1;

  SDL_RenderCopy(renderer, texGameOver, NULL, goPos);

  delete goPos;

  while(1) {
    if(SDL_PollEvent(event)) {
      switch(event->type) {
        case SDL_QUIT: {
            exit(0);
          }
          break;
        default:
          {}
          break;
      }
    }
    SDL_RenderPresent(renderer);
  }
}

void randomTile() {
  available.clear();
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      if(!board[i][j]) {
        tmpBoardCellPosition.x = i;
        tmpBoardCellPosition.y = j;
        available.push_back(tmpBoardCellPosition);
      }
    }
  }
  if(available.size() > 0) {
    int cellPos = rand() % available.size();
    int cellVal = (rand() % 2 + 1) * 2;
    board[available[cellPos].x][available[cellPos].y] = cellVal;
  } else {
    gameOver();
  }
}

void draw() {
  SDL_SetRenderDrawColor(renderer, gameBackgroundColor.r, gameBackgroundColor.g, gameBackgroundColor.b, gameBackgroundColor.a);
  SDL_RenderClear(renderer);
  int winW, winH;
  SDL_GetWindowSize(window, &winW, &winH);
  int boardW = 15 * 2 + 121 * 4;
  int boardH = 15 * 2 + 121 * 4;
  int xOffset = (winW - boardW) / 2;
  int yOffset = (winH - boardH) / 2;
  SDL_Rect *drawPos = new SDL_Rect;
  drawPos->w=boardW;
  drawPos->h=boardH;
  drawPos->x=xOffset;
  drawPos->y=yOffset;
  SDL_SetRenderDrawColor(renderer, boardBackgroundColor.r, boardBackgroundColor.g, boardBackgroundColor.b, boardBackgroundColor.a);
  SDL_RenderFillRect(renderer,drawPos);
  drawPos->h = drawPos->w = 107;
  for(int i = 0; i < 4; i++) {
    drawPos->y = 15 + 121 * i + yOffset;
    for(int j = 0; j < 4; j++) {
      drawPos->x = 15 + 121 * j + xOffset;
      if(board[i][j] == 2) {
        SDL_RenderCopy(renderer, tex2, NULL, drawPos);
      } else if(board[i][j] == 4) {
        SDL_RenderCopy(renderer, tex4, NULL, drawPos);
      } else if(board[i][j] == 8) {
        SDL_RenderCopy(renderer, tex8, NULL, drawPos);
      } else if(board[i][j] == 16) {
        SDL_RenderCopy(renderer, tex16, NULL, drawPos);
      } else if(board[i][j] == 32) {
        SDL_RenderCopy(renderer, tex32, NULL, drawPos);
      } else if(board[i][j] == 64) {
        SDL_RenderCopy(renderer, tex64, NULL, drawPos);
      } else if(board[i][j] == 128) {
        SDL_RenderCopy(renderer, tex128, NULL, drawPos);
      } else if(board[i][j] == 256) {
        SDL_RenderCopy(renderer, tex256, NULL, drawPos);
      } else if(board[i][j] == 512) {
        SDL_RenderCopy(renderer, tex512, NULL, drawPos);
      } else if(board[i][j] == 1024) {
        SDL_RenderCopy(renderer, tex1024, NULL, drawPos);
      } else if(board[i][j] == 2048) {
        SDL_RenderCopy(renderer, tex2048, NULL, drawPos);
      }
    }
  }
  delete drawPos;
}

void logic(int dir) {
  if(dir == Direction::up) {
    upLogic();
  } else if(dir == Direction::down) {
    downLogic();
  } else if(dir == Direction::left) {
    leftLogic();
  } else if(dir == Direction::right) {
    rightLogic();
  }
}

bool isWin() {
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      if(board[i][j] == winValue) {
        return true;
      }
    }
  }
  return false;
}

void winGame() {
  int winW, winH;
  puts("GAME WON");
  SDL_GetWindowSize(window, &winW, &winH);
  SDL_Rect *goPos = new SDL_Rect;

  goPos->w = sGameWon->w;
  goPos->h = sGameWon->h;
  goPos->x = (winW - goPos->w) >> 1;
  goPos->y = (winH - goPos->h) >> 1;

  SDL_RenderCopy(renderer, texGameWon, NULL, goPos);

  delete goPos;

  while(1) {
    if(SDL_PollEvent(event)) {
      switch(event->type) {
        case SDL_QUIT: {
            exit(0);
          }
          break;
        default:
          {}
          break;
      }
    }
    SDL_RenderPresent(renderer);
  }
}

void exitGame() {
  delete event;
  SDL_FreeSurface(sGameOver);
  SDL_FreeSurface(sGameWon);
  SDL_DestroyTexture(tex2);
  SDL_DestroyTexture(tex4);
  SDL_DestroyTexture(tex8);
  SDL_DestroyTexture(tex16);
  SDL_DestroyTexture(tex32);
  SDL_DestroyTexture(tex64);
  SDL_DestroyTexture(tex128);
  SDL_DestroyTexture(tex256);
  SDL_DestroyTexture(tex512);
  SDL_DestroyTexture(tex1024);
  SDL_DestroyTexture(tex2048);
  SDL_DestroyTexture(texGameOver);
  SDL_DestroyTexture(texGameWon);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}

int main(int argc, char** argv) {
  puts("sdl init");
  SDL_Init(SDL_INIT_EVERYTHING);
  atexit(exitGame);

  SDL_CreateWindowAndRenderer(1280, 720, SDL_WINDOW_SHOWN, &window, &renderer);
  SDL_SetWindowTitle(window, "2048 - SonnyStar");

  puts("load textures");
  // 2
  s = SDL_LoadBMP("media/bmp/2.bmp");
  tex2 = SDL_CreateTextureFromSurface(renderer, s);
  SDL_FreeSurface(s);
  // 4
  s = SDL_LoadBMP("media/bmp/4.bmp");
  tex4 = SDL_CreateTextureFromSurface(renderer, s);
  SDL_FreeSurface(s);
  // 8
  s = SDL_LoadBMP("media/bmp/8.bmp");
  tex8 = SDL_CreateTextureFromSurface(renderer, s);
  SDL_FreeSurface(s);
  // 16
  s = SDL_LoadBMP("media/bmp/16.bmp");
  tex16 = SDL_CreateTextureFromSurface(renderer, s);
  SDL_FreeSurface(s);
  // 32
  s = SDL_LoadBMP("media/bmp/32.bmp");
  tex32 = SDL_CreateTextureFromSurface(renderer, s);
  SDL_FreeSurface(s);
  // 64
  s = SDL_LoadBMP("media/bmp/64.bmp");
  tex64 = SDL_CreateTextureFromSurface(renderer, s);
  SDL_FreeSurface(s);
  // 128
  s = SDL_LoadBMP("media/bmp/128.bmp");
  tex128 = SDL_CreateTextureFromSurface(renderer, s);
  SDL_FreeSurface(s);
  // 256
  s = SDL_LoadBMP("media/bmp/256.bmp");
  tex256 = SDL_CreateTextureFromSurface(renderer, s);
  SDL_FreeSurface(s);
  // 512
  s = SDL_LoadBMP("media/bmp/512.bmp");
  tex512 = SDL_CreateTextureFromSurface(renderer, s);
  SDL_FreeSurface(s);
  // 1024
  s = SDL_LoadBMP("media/bmp/1024.bmp");
  tex1024 = SDL_CreateTextureFromSurface(renderer, s);
  SDL_FreeSurface(s);
  // 2048
  s = SDL_LoadBMP("media/bmp/2048.bmp");
  tex2048 = SDL_CreateTextureFromSurface(renderer, s);
  SDL_FreeSurface(s);
  // Game Over
  sGameOver = SDL_LoadBMP("media/bmp/gameOver.bmp");
  texGameOver = SDL_CreateTextureFromSurface(renderer, sGameOver);
  // Game Won
  sGameWon = SDL_LoadBMP("media/bmp/gameWon.bmp");
  texGameWon = SDL_CreateTextureFromSurface(renderer, sGameWon);

  randomTile();
  draw();

  puts("game loop");
  event = new SDL_Event;
  while(1) {
    if(SDL_PollEvent(event)) {
      switch(event->type) {
        case SDL_KEYDOWN: {
            switch(event->key.keysym.scancode) {
              case SDL_SCANCODE_UP: {
                  logic(Direction::up);
                  randomTile();
                  draw();
                }
                break;
              case SDL_SCANCODE_DOWN: {
                  logic(Direction::down);
                  randomTile();
                  draw();
                }
                break;
              case SDL_SCANCODE_LEFT: {
                  logic(Direction::left);
                  randomTile();
                  draw();
                }
                break;
              case SDL_SCANCODE_RIGHT: {
                  logic(Direction::right);
                  randomTile();
                  draw();
                }
                break;
              default:
                {}
                break;
            }
          }
          break;
        case SDL_QUIT: {
            exit(0);
          }
          break;
        default:
          {}
          break;
      }
    }
    if(isWin()) {
      winGame();
    }
    SDL_RenderPresent(renderer);
  }
  return 0;
}
