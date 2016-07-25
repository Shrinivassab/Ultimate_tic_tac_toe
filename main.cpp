
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>

#define DEPTH 81
#define INF 10000

		 int autoplay = 1;

void wait(const char *str = NULL)
{
	char ch;
	printf("[%s]", str == NULL ? "Wait" : str);
	scanf("%c", &ch);
}

FILE *fp = fopen("out.txt", "w");

class gameState
{
public:
	int state[3][3][3][3];	// r c i j
	int score[3][3][2], lsr, lsc;

	gameState()
	{
		memset(state, 0, sizeof(state));
		memset(score, 0, sizeof(score));
		lsr = lsc = -1;
	}

	int over()
	{
		for (int i = 0; i<3; i++)
		{
			for (int j = 0; j<3; j++)
			{
				for (int r = 0; r<3; r++)
				{
					for (int c = 0; c<3; c++)
					{
						if (state[r][c][i][j] == 0)
							return 0;
					}
				}
			}
		}
		return 1;
	}

	void totalScore(int *p1, int *p2)
	{
		int total[2] = { 0, 0 };
		for (int i = 0; i<3; i++)
		{
			for (int j = 0; j<3; j++)
			{
				total[0] += score[i][j][0];
				total[1] += score[i][j][1];
			}
		}
		*p1 = total[0];
		*p2 = total[1];
	}

	void eval(int r, int c)
	{
		score[r][c][0] = score[r][c][1] = 0;
		for (int i = 0; i<3; i++)
		{
			int flag = 1, cur = state[r][c][i][0];
			for (int j = 1; j<3; j++)
			{
				if (cur != state[r][c][i][j])
					flag = 0;
			}
			if (flag && cur != 0)
				score[r][c][cur == -1 ? 0 : 1]++;
		}
		for (int i = 0; i<3; i++)
		{
			int flag = 1, cur = state[r][c][0][i];
			for (int j = 1; j<3; j++)
			{
				if (cur != state[r][c][j][i])
					flag = 0;
			}
			if (flag && cur != 0)
				score[r][c][cur == -1 ? 0 : 1]++;
		}
		if (state[r][c][0][0] == state[r][c][1][1] && state[r][c][1][1] == state[r][c][2][2] && state[r][c][2][2] != 0)
			score[r][c][state[r][c][1][1] == -1 ? 0 : 1]++;
		if (state[r][c][0][2] == state[r][c][1][1] && state[r][c][1][1] == state[r][c][2][0] && state[r][c][2][2] != 0)
			score[r][c][state[r][c][1][1] == -1 ? 0 : 1]++;
	}

	int isEmpty(int r, int c)
	{
		int tot = 0;
		for (int i = 0; i<3; i++)
		{
			for (int j = 0; j<3; j++)
			{
				tot += state[r][c][i][j] != 0;
			}
		}
		return !tot;
	}

	int isFull(int r, int c)
	{
		int tot = 0;
		for (int i = 0; i<3; i++)
		{
			for (int j = 0; j<3; j++)
			{
				tot += state[r][c][i][j] != 0;
			}
		}
		return tot == 9;
	}

	int alphabeta(int player, gameState *game, int alpha, int beta, int depth)
	{
		if (game->over() || depth <= 0)
		{
			int p1, p2;
			totalScore(&p1, &p2);
			return p1 - p2;
		}

		gameState *children[9];
		int c = 0;
		fprintf(fp, "[%d, %d] ->", lsr, lsc);
		for (int i = 0; i<3; i++)
		{
			for (int j = 0; j<3; j++)
			{
				if (!game->state[lsr][lsc][i][j])
				{
					children[c] = new gameState;
					memcpy(children[c]->state, game->state, sizeof(game->state));
					memcpy(children[c]->score, game->score, sizeof(game->score));
					children[c]->state[lsr][lsc][i][j] = player;
					children[c]->eval(lsr, lsc);
					//if(i==2 && j==2 && player==1 && lsr==0 && lsc==0)
					//	printf("<%d, %d, %d, %d, %d>", children[c]->state[0][0][0][0], children[c]->state[0][0][1][1], children[c]->state[0][0][2][2], score[lsr][lsc][0], children[c]->score[lsr][lsc][1]);
					children[c]->lsr = i;
					children[c]->lsc = j;
					c++;
				}
			}
		}
		fprintf(fp, "\n");
		if (c == 0)
			return 0;

		if (player == 1)
		{
			for (int k = 0; k<c; k++)
			{
				int s = alphabeta(-player, children[k], alpha, beta, depth - 1);
				if (s >= alpha)
					alpha = s;
				if (alpha >= beta)
					break;
			}
			for (int i = 0; i<c; i++)
				delete children[i];
			return alpha;
		}
		else
		{
			for (int k = 0; k<c; k++)
			{
				int s = alphabeta(-player, children[k], alpha, beta, depth - 1);
				//if(s!=0) printf("[%d, %d]=%d\n", children[k]->lsr, children[k]->lsc, s);
				if (s<beta)
					beta = s;
				if (alpha >= beta)
					break;
			}
			for (int i = 0; i<c; i++)
				delete children[i];
			//wait("Goal");
			return beta;
		}
		fprintf(fp, "\n");
	}

	void bestOption(int player, int lsr, int lsc, int *r, int *c)
	{
		if (
			int max = -INF *2;
		int arr[9][2], xc = 0;
		for (int i = 0; i<3; i++)
		{
			for (int j = 0; j<3; j++)
			{
				if (!state[lsr][lsc][i][j])
				{
					state[lsr][lsc][i][j] = player;
					eval(lsr, lsc);
					int s;
					if (player != 1)
						s = score[lsr][lsc][0] - score[lsr][lsc][1];
					else
						s = score[lsr][lsc][1] - score[lsr][lsc][0];
					s -= alphabeta(player, this, -INF, INF, DEPTH);
					//printf("[%d, %d] = %d\n", i, j, s);
					//wait();
					if (s >= max)
					{
						if (s>max)
						{
							max = s;
							xc = 0;
						}
						arr[xc][0] = i;
						arr[xc++][1] = j;
					}
					state[lsr][lsc][i][j] = 0;
				}
			}
		}
		int done = 0;
		for (int i = 0; i<xc; i++)
		{
			eval(lsr, lsc);
			int y[2] = { score[lsr][lsc][0], score[lsr][lsc][1] };
			state[lsr][lsc][arr[i][0]][arr[i][1]] = -player;
			eval(lsr, lsc);
			state[lsr][lsc][arr[i][0]][arr[i][1]] = 0;
			if ((player == 1 && y[0]<score[lsr][lsc][0]) || (player == -1 && y[1]<score[lsr][lsc][1]))
			{
				*r = arr[i][0];
				*c = arr[i][1];
				i = xc;
				done = 1;
			}
			score[lsr][lsc][0] = y[0];
			score[lsr][lsc][1] = y[1];
		}
		if (!done)
		{
			int gr = rand() % xc;
			*r = arr[gr][0];
			*c = arr[gr][1];
		}
	}

	void print(int xr = -1, int xc = -1)
	{
		system("clear");
		printf("\x1B[31m");
		for (int k = 0; k<37; k++)
		{
			if (k >= 36 * float(xc) / 3 && k <= 36 * float(xc + 1) / 3 && xr == 0)
				printf("\x1B[32m");
			if (k == 0 || k == 24 || k == 12 || k == 36)
				printf("+");
			else
				printf("-");
			if (!(k >= 36 * float(xc) / 3 && k <= 36 * float(xc + 1) / 3) && xr == 0)
				printf("\x1B[31m");
		}
		printf("\x1B[0m");
		printf("\n");
		for (int r = 0; r<3; r++)
		{
			for (int i = 0; i<3; i++)
			{
				if (xr == r && xc == 0)
					printf("\x1B[32m");
				else
					printf("\x1B[31m");
				printf("|");
				//if(xr==r && xc==0)	
				printf("\x1B[0m");
				for (int c = 0; c<3; c++)
				{
					for (int j = 0; j<3; j++)
					{
						printf(" %c ", state[r][c][i][j] == 1 ? 'X' : (state[r][c][i][j] == 0 ? ' ' : 'O'));
						printf("\x1B[31m");
						if ((xr == r && xc == c) || (xc == c + 1 && j == 2 && xr == r))
							printf("\x1B[32m");
						printf("|");
						if ((xr == r && xc == c) || (xc == c + 1 && j == 2))
							printf("\x1B[31m");
						printf("\x1B[0m");
					}
				}
				printf("\n");
				if (i == 2)
					printf("\x1B[31m");
				for (int k = 0; k<37; k++)
				{
					printf("\x1B[31m");
					if (k >= 36 * float(xc) / 3 && k <= 36 * float(xc + 1) / 3 && i == 2 && (r == xr || r == xr - 1))
						printf("\x1B[32m");
					if ((k == 0 || k == 24 || k == 12 || k == 36) && i == 2)
						printf("+");
					else
						printf("-");
					if (!(k >= 36 * float(xc) / 3 && k <= 36 * float(xc + 1) / 3 && i == 2 && (r == xr || r == xr - 1)))
						printf("\x1B[31m");
				}
				if (i == 2)
					printf("\x1B[0m");
				printf("\n");
			}
		}
	}

	void scorer(int *p1, int *p2)
	{
		for (int i = 0; i<3; i++)
			for (int j = 0; j<3; j++)
				eval(i, j);
		int total[2] = { 0, 0 };
		for (int i = 0; i<3; i++)
		{
			for (int j = 0; j<3; j++)
			{
				total[0] += score[i][j][0];
				total[1] += score[i][j][1];
			}
		}
		*p1 = total[0];
		*p2 = total[1];
	}
};

int main()
{
	srand(time(NULL));
	/*
	gameState g;
	g.state[0][0][0][0]=1;
	g.state[0][0][1][1]=1;
	g.state[2][2][0][0]=-1;
	g.state[1][1][1][1]=-1;
	int x1, x2;
	g.bestOption(1, 0, 0, &x1, &x2);
	printf("%d %d\n", x1, x2);
	return 0;*/

	gameState game;
	//printf("Go first? (y/n) ");
	char ch; //scanf("%c", &ch); getchar();
	ch = 'n';
	int player = 1;
	if (ch == 'y' || ch == 'Y')
		player *= 1;
	else
		player *= -1;

	int r, c;

	if (player == -1)
	{
		int r, c, i, j;
		r = rand() % 3;
		c = rand() % 3;
		i = rand() % 3;
		j = rand() % 3;
		game.state[r][c][i][j] = player;
		game.lsr = i; game.lsc = j;
		player = 1;
	}

	while (!game.over())
	{
		if (player == 1)
		{
			if (!autoplay)
			{
				while ((game.lsr<0 && game.lsc<0) || (game.lsr>3 && game.lsc>3))
				{
					game.print();
					//printf("<Row> <Col> ");
					//scanf("%d %d", &game.lsr, &game.lsc);
					game.lsr = 0; game.lsc = 0;
				}
				do
				{
					game.print(game.lsr, game.lsc);
					int p1, p2;
					game.scorer(&p1, &p2);
					printf("\n\nScore:\t[O] - %d\n\t[X] - %d\n\n\n", p1, p2);
					printf("<Row> <Col> ");
					scanf("%d %d", &r, &c);
					printf("\n\nThinking...\n\n");
					//r=1; c=1;
				} while (game.state[game.lsr][game.lsc][r][c]);
			}
			else
			{
				game.print(game.lsr, game.lsc);
				int p1, p2;
				game.scorer(&p1, &p2);
				printf("\n\nScore:\t[O] - %d\n\t[X] - %d\n\n\n", p1, p2);
				game.bestOption(player, game.lsr, game.lsc, &r, &c);
			}
		}
		if (player == -1)
		{
			game.print(game.lsr, game.lsc);
			int p1, p2;
			game.scorer(&p1, &p2);
			printf("\n\nScore:\t[O] - %d\n\t[X] - %d\n\n\n", p1, p2);
			game.bestOption(player, game.lsr, game.lsc, &r, &c);
		}
		game.state[game.lsr][game.lsc][r][c] = player;
		player *= -1;
		game.lsr = r;
		game.lsc = c;
		//wait("Go?");
	}

	return 0;
}
