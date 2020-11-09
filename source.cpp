#include <bits/stdc++.h>
#include <bits/glut.h>
using namespace std;
typedef long long LL;
#define mp make_pair
#define CHECK(x) if(!(x)) return false;
#define CHECKRET(x, y) if(!(x)) return (y);
#define SKIP(x) if((x)) continue;

typedef pair<int, int> sq;
typedef pair<sq, sq> mv;

const mv INVALID_MOVE = mp(mp(-1, -1), mp(-1, -1));

struct state {
  char board[8][8];
  bool whiteToMove;
  bool whiteCanCastle, blackCanCastle;
  mv prevMove;

  void init() {
    memset(board, 0, sizeof board);
    whiteToMove = whiteCanCastle = blackCanCastle = true;
    prevMove = INVALID_MOVE;
    for (int i = 0; i < 8; ++i) {
      board[1][i] = 'P';
      board[6][i] = 'p';
    }
    board[0][0] = board[0][7] = 'R';
    board[7][0] = board[7][7] = 'r';
    board[0][1] = board[0][6] = 'N';
    board[7][1] = board[7][6] = 'n';
    board[0][2] = board[0][5] = 'B';
    board[7][2] = board[7][5] = 'b';
    board[0][3] = 'Q';
    board[7][3] = 'q';
    board[0][4] = 'K';
    board[7][4] = 'k';
  }

  bool isWhitePiece(char piece) {
    return piece >= 'A' && piece <= 'Z';
  }

  bool isValidSquare(int i, int j) {
    CHECK(i >= 0 && i < 8);
    CHECK(j >= 0 && j < 8);
    return true;
  }

#define PUSHMOVE res.push_back(mp(mp(i, j), mp(ni, nj)));

  void allPieceMoves(int i, int j, char piece, vector<mv>& res) {
    if (board[i][j] == 0) return;
    if (isWhitePiece(piece) != whiteToMove) return;
    int ni, nj, dr;
    switch (piece) {
    case 'n':
    case 'N':
      for (int di = -2; di <= 2; ++di)
        for (int dj = -2; dj <= 2; ++dj) {
          SKIP(abs(di) + abs(dj) != 3);
          ni = i + di, nj = j + dj;
          SKIP(!isValidSquare(ni, nj));
          SKIP(board[ni][nj] != 0 && whiteToMove == isWhitePiece(board[ni][nj]));
          PUSHMOVE;
        }
      break;
    case 'b':
    case 'B':
      for (int di = -1; di <= 1; di += 2)
        for (int dj = -1; dj <= 1; dj += 2) {
          SKIP(abs(di) + abs(dj) != 2);
          for (int d = 1; d < 8; ++d) {
            ni = i + di * d, nj = j + dj * d;
            if (!isValidSquare(ni, nj)) break;
            if (board[ni][nj] != 0 && whiteToMove == isWhitePiece(board[ni][nj])) break;
            PUSHMOVE;
            if (board[ni][nj] != 0 && whiteToMove != isWhitePiece(board[ni][nj])) break;
          }
        }
      break;
    case 'r':
    case 'R':
      for (int di = -1; di <= 1; ++di)
        for (int dj = -1; dj <= 1; ++dj) {
          SKIP(abs(di) + abs(dj) != 1);
          for (int d = 1; d < 8; ++d) {
            ni = i + di * d, nj = j + dj * d;
            if (!isValidSquare(ni, nj)) break;
            if (board[ni][nj] != 0 && whiteToMove == isWhitePiece(board[ni][nj])) break;
            PUSHMOVE;
            if (board[ni][nj] != 0 && whiteToMove != isWhitePiece(board[ni][nj])) break;
          }
        }
      break;
    case 'q':
      allPieceMoves(i, j, 'b', res);
      allPieceMoves(i, j, 'r', res);
      break;
    case 'Q':
      allPieceMoves(i, j, 'B', res);
      allPieceMoves(i, j, 'R', res);
      break;
    case 'p':
    case 'P':
      dr = (whiteToMove ? 1 : (-1));
      ni = i + dr, nj = j;
      if (isValidSquare(ni, nj) && board[ni][nj] == 0) {
        PUSHMOVE;
        ni = i + 2 * dr, nj = j;
        if (isValidSquare(ni, nj) && board[ni][nj] == 0)
          PUSHMOVE;
      }
      for (int dj = -1; dj <= 1; dj += 2) {
        ni = i + dr, nj = j + dj;
        SKIP(!isValidSquare(ni, nj));
        SKIP(board[ni][nj] == 0);
        SKIP(whiteToMove == isWhitePiece(board[ni][nj]));
        PUSHMOVE;
      }
      break;
    case 'k':
    case 'K':
      for (int di = -1; di <= 1; ++di)
        for (int dj = -1; dj <= 1; ++dj) {
          SKIP(di == 0 && dj == 0);
          ni = i + di, nj = j + dj;
          SKIP(!isValidSquare(ni, nj));
          SKIP(whiteToMove == isWhitePiece(board[ni][nj]));
          PUSHMOVE;
        }
      break;
    }
  }

  void allAvailableMoves(vector<mv>& res) {
    for (int i = 0; i < 8; ++i)
      for (int j = 0; j < 8; ++j)
        allPieceMoves(i, j, board[i][j], res);
  }

  int eval() {
    int controlled[8][8];
    memset(controlled, 0, sizeof controlled);
    vector<mv> allMoves;
    allAvailableMoves(allMoves);
    whiteToMove = !whiteToMove;
    allAvailableMoves(allMoves);
    whiteToMove = !whiteToMove;
    for (const auto& m : allMoves)
      controlled[m.second.first][m.second.second] += (isWhitePiece(board[m.first.first][m.first.second]) ? 1 : (-1));
    int s = 0;
    for (int i = 0; i < 8; ++i)
      for (int j = 0; j < 8; ++j) {
        switch (board[i][j]) {
        case 'p':
          s -= 100;
          break;
        case 'P':
          s += 100;
          break;
        case 'n':
          s -= 300;
          break;
        case 'N':
          s += 300;
          break;
        case 'b':
          s -= 300;
          break;
        case 'B':
          s += 300;
          break;
        case 'r':
          s -= 500;
          break;
        case 'R':
          s += 500;
          break;
        case 'q':
          s -= 900;
          break;
        case 'Q':
          s += 900;
          break;
        case 'k':
          s -= 10000000;
          break;
        case 'K':
          s += 10000000;
          break;
        case 0:
          if (controlled[i][j] > 0) s += 10;
          else s -= 10;
          break;
        }
      }
    if (!whiteToMove) s = -s;
    return s;
  }

  void applyMove(mv which) {
    board[which.second.first][which.second.second] = board[which.first.first][which.first.second];
    board[which.first.first][which.first.second] = 0;
    whiteToMove = !whiteToMove;
  }

  pair<int, mv> DFS(int depth) {
    if (depth <= 0) return mp(eval(), INVALID_MOVE);
    vector<mv> cands;
    allAvailableMoves(cands);
    pair<int, mv> best = mp(INT_MIN, INVALID_MOVE);
    for (const auto& m : cands) {
      state st;
      memcpy(&st, this, sizeof st);
      st.applyMove(m);
      best = max(best, mp(-st.DFS(depth - 1).first, m));
    }
    return best;
  }

  bool validateMove(mv m) {
    vector<mv> allMoves;
    allAvailableMoves(allMoves);
    return find(allMoves.begin(), allMoves.end(), m) != allMoves.end();
  }
};

state* cur;

void printBoard() {
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j)
      putchar(cur->board[7 - i][j] == 0 ? '.' : cur->board[7 - i][j]);
    puts("");
  }
}

sq getSquare() {
  char rank, column;
  scanf(" %c %c", &column, &rank);
  if (!
    (column >= 'a' && column <= 'h' && rank >= '1' && rank <= '8')
    ) {
    puts("Invalid square!");
    return getSquare();
  }
  return mp(rank - '1', column - 'a');
}

mv getPlayerMove() {
  sq f = getSquare();
  sq t = getSquare();
  mv m = mp(f, t);
  if (!cur->validateMove(m)) {
    puts("INVALID MOVE!");
    return getPlayerMove();
  }
  return m;
}

void KeyBoardFunc(unsigned char key, int x, int y) {
  if (key == 27)
    exit(0);
  glutPostRedisplay();
}

double pi = atan(1) * 4;

void drawPolygonInSquare(double l, double u, int sides, bool isWhite) {
  double rat = 1.0 / 10.0;
  double dv = 2 * pi / sides;
  for (int mode = 0; mode < 2; ++mode) {
    if (mode == 0) {
      glPolygonMode(GL_FRONT, GL_LINE);
      if (isWhite) glColor3d(0.0, 0.0, 0.0);
      else glColor3d(1.0, 1.0, 1.0);
    }
    else {
      glPolygonMode(GL_FRONT, GL_FILL);
      if (isWhite) glColor3d(0.6, 1.0, 1.0);
      else glColor3d(0.4, 0.0, 0.0);
    }
    double alpha = 0.0;
    glBegin(GL_POLYGON); {
      for (int i = 0; i < sides; ++i, alpha += dv)
        glVertex2d(cos(alpha) * rat + l, sin(alpha) * rat + u);
    } glEnd();
  }
}

void drawSquare(int i, int j) {
  if ((abs(i) + abs(j)) % 2 == 0) glColor3d(0.2, 0.2, 0.2);
  else glColor3d(0.8, 0.8, 0.8);
  double dv = 2.0 / 8;
  double l = -1 + j * dv, r = -1 + (j + 1) * dv,
    u = -1 + i * dv, d = -1 + (i + 1) * dv;
  glPolygonMode(GL_FRONT, GL_FILL);
  glBegin(GL_POLYGON); {
    glVertex2d(l, u);
    glVertex2d(l, d);
    glVertex2d(r, d);
    glVertex2d(r, u);
  } glEnd();
  switch (cur->board[i][j]) {
  case 'p':
  case 'P':
    drawPolygonInSquare((l + r) / 2, (d + u) / 2, 3, cur->isWhitePiece(cur->board[i][j]));
    break;
  case 'n':
  case 'N':
    drawPolygonInSquare((l + r) / 2, (d + u) / 2, 4, cur->isWhitePiece(cur->board[i][j]));
    break;
  case 'b':
  case 'B':
    drawPolygonInSquare((l + r) / 2, (d + u) / 2, 5, cur->isWhitePiece(cur->board[i][j]));
    break;
  case 'r':
  case 'R':
    drawPolygonInSquare((l + r) / 2, (d + u) / 2, 6, cur->isWhitePiece(cur->board[i][j]));
    break;
  case 'q':
  case 'Q':
    drawPolygonInSquare((l + r) / 2, (d + u) / 2, 9, cur->isWhitePiece(cur->board[i][j]));
    break;
  case 'k':
  case 'K':
    drawPolygonInSquare((l + r) / 2, (d + u) / 2, 50, cur->isWhitePiece(cur->board[i][j]));
    break;
  default:
    break;
  }
}

void Display() {
  glClear(GL_COLOR_BUFFER_BIT);
  for (int i = 0; i < 8; ++i)
    for (int j = 0; j < 8; ++j)
      drawSquare(i, j);
  glFlush();
}

deque<sq> clicks;

void MouseFunc(int button, int state, int x, int y) {
  if (state == 0) return;
  int wp[4];
  glGetIntegerv(GL_VIEWPORT, wp);
  int w = wp[2], h = wp[3];
  double xx = 1.0 * x / w, yy = 1.0 * y / h;
  double dv = 1.0 / 8;
  int xxx = xx / dv, yyy = yy / dv;
  clicks.push_back(mp(7 - yyy, xxx));
  if (clicks.size() < 2) return;
  mv m = mp(clicks[0], clicks[1]);
  clicks.pop_front(); clicks.pop_front();
  if (!cur->validateMove(m)) {
    puts("Invalid move!");
    return;
  }
  cur->applyMove(m);
  Display();
  puts("I am thinking...");
  m = cur->DFS(4).second;
  cur->applyMove(m);
  glutPostRedisplay();
}

void Reshape(int w, int h) {
  glViewport(0, 0, w, h);
}

int main(int argc, char** argv) {
  cur = new state();
  cur->init();
  /*
  while (1) {
    printBoard();
    puts("What's your move?");
    mv m = getPlayerMove();
    cur->applyMove(m);
    m = cur->DFS(3).second;
    cur->applyMove(m);
  }
  */
  glutInit(&argc, argv);
  glutInitWindowSize(800, 800);
  glutInitWindowPosition(500, 100);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutCreateWindow(argv[0]);

  glLineWidth(4);

  glutReshapeFunc(Reshape);
  glutKeyboardFunc(KeyBoardFunc);
  glutMouseFunc(MouseFunc);
  glutDisplayFunc(Display);

  glutMainLoop();

  return 0;
}
