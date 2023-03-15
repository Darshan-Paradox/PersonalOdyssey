#define LEN 5

struct Player
{
	int level;
	double coins;
	int tasks;
	int curr_tasks;
	int streak;
	double rating[5];
};

typedef struct Player Player;

char *props[5] = {"Agility", "Strength",
				  "Academics", "Creativity",
				  "Critical Analysis"};

void progress_bar(Window *win, int y, int x, int n_bars)
{
	wattron(win->win, COLOR_PAIR(3));
	mvwprintw(win->win, y, x, "%*c", n_bars, '_');
	wattroff(win->win, COLOR_PAIR(3));
}

void disp_stats(Window *win, double *vals)
{
	wattron(win->win, COLOR_PAIR(2));

	for (int i = 0; i < LEN; i++)
		mvwprintw(win->win, 2*i + 1, 1, "%s:: %.1f%%", props[i], vals[i]*2);

	wattroff(win->win, COLOR_PAIR(2));

	for (int i = 0; i < LEN; i++)
		progress_bar(win, 2*i + 2, 1, (int)vals[i]);
}

void disp_info(Window *win, Player *player)
{
	time_t t;
	time(&t);

	wattron(win->win, A_BOLD| COLOR_PAIR(5));

	mvwprintw(win->win, 1, 1, "DATE:: %s", ctime(&t));
	mvwprintw(win->win, 3, 1, "LEVEL:: %d", player->level);
	mvwprintw(win->win, 5, 1, "COINS:: %.1f", player->coins);
	mvwprintw(win->win, 7, 1, "STREAK:: %d", player->streak);
	mvwprintw(win->win, 9, 1, "# (TASK):: %d", player->tasks);

	wattroff(win->win, A_BOLD | COLOR_PAIR(5));
}

void disp_tasks(Window *win, char task[5][256], int done[5], int task_no)
{
	for (int i = 0; i < task_no; i++)
	{
		if (done[i])
			wattron(win->win, COLOR_PAIR(8));
		else
			wattron(win->win, COLOR_PAIR(7));

		mvwprintw(win->win, 4 + 3*i, 1, "|>%s", task[i]);

		if (done[i])
			wattroff(win->win, COLOR_PAIR(8));
		else
			wattroff(win->win, COLOR_PAIR(7));
	}
}

void disp_items(Window *win)
{
	char *items[] = {"Coupon: $5",
					 "Coupon: $20",
					 "Coupon: $100",
					 "Free time @1hrs: $10",
					 "Free time @2hrs: $25",
					 "Outing (Sainikpuri): $250",
					 "Outing (Hyderabad): $300"};

	for (int i = 0; i < 7; i++)
	{
		mvwprintw(win->win, 2 + 3*i, 1, "||>%s", items[i]);
	}
}
