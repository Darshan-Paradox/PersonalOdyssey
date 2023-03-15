#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <curses.h>
#include <sqlite3.h>

//including all the self-defined header files
#include "tasks.h"
#include "win.h"
#include "player.h"

int main()
{
	time_t date;
	time(&date);

	/*
	 * Run the python script tasks.py, which automatically generates
	 * randomised task and push it to database.
	 *
	 * It also calculates other values like stats, level and coins.
	 * 
	 * Though system calls are unsafe to use, but a constant string
	 * is provided as input.
	 * Also this is a very personal level project
	 * that was only going to be use by me.
	*/
	system("python3 ./scripts/tasks.py");

	int row, col;
	//initialise the stdscr and allocate memory
	initscr();
	//getting the size of the window
	getmaxyx(stdscr, row, col);

	//initial settings
	MEVENT event;
	cbreak();
	noecho();
	start_color();
	keypad(stdscr, true);
	mousemask(ALL_MOUSE_EVENTS, NULL);

	//initialise all required color pairs
	init_pair(1, COLOR_BLUE, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_GREEN);
	init_pair(4, COLOR_RED, COLOR_BLACK);
	init_pair(5, COLOR_CYAN, COLOR_BLACK);
	init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(7, COLOR_YELLOW, COLOR_BLACK);
	init_pair(8, COLOR_YELLOW, COLOR_RED);

	//title of main screen
	title(NULL, "===PERSONAL ODYSSEY===", row, col);

	//Window with all the properties
	Window *stats = create_win(12, col/2 - 1, 4, 1);

	box(stats->win, 0, 0);
	wrefresh(stats->win);

	//Window with level, streak and other info
	Window *info = create_win(12, col/3, 4, col/2);

	box(info->win, 0, 0);
	wrefresh(info->win);

	//Window with progress bar to show streaks
	Window *streak = create_win(12, col/6, 4, 5*col/6);

	box(streak->win, 0, 0);
	wrefresh(streak->win);
	

	//Window with today's task
	Window *tasks = create_win(row - 28, 2*col/3 - 1, 17, 1);

	//title ==TASKS==
	wattron(tasks->win, A_BOLD | COLOR_PAIR(4));
	title(tasks, "TASKS", 0, 2*col/3);
	wattroff(tasks->win, A_BOLD | COLOR_PAIR(4));

	box(tasks->win, 0, 0);
	wrefresh(tasks->win);

	//Prompt window for all the user text inputs
	Window *prompt = create_win(5, 2*col/3 - 1, row - 11, 1);
	box(prompt->win, 0, 0);

	//title PROMPT
	wattron(prompt->win, A_BOLD | COLOR_PAIR(1));
	title(prompt, "PROMPT", 0, 2*col/3);
	wattroff(prompt->win, A_BOLD | COLOR_PAIR(1));
	wrefresh(prompt->win);

	//Store window, used to purchase items using coins
	Window *store = create_win(row - 23, col/3, 17, 2*col/3);

	//store_items and its cost values
	int store_items[] = {5, 20, 100, 10, 25, 250, 300};

	disp_items(store);

	box(store->win, 0, 0);

	//title STORE
	wattron(store->win, A_BOLD);
	title(store, "STORE", 0, col/3);
	wattroff(store->win, A_BOLD);

	wrefresh(store->win);

	//Window for surprise task
	Window *sur_tasks = create_win(6, col - 1, row - 6, 1);

	/*
	 * getting surprise task from another python script
	 * is_surprise checks if surprise tasks exists or not
	 * completed_surprise check if user has completed the surprise or not
	*/
	char surprise_task[256];
	int is_surprise = get_surprise(surprise_task, 256);
	int completed_surprise = 0;

	if (is_surprise)
		completed_surprise = get_completed(5);

	//displaying surprise task based on given conditions
	if (is_surprise)
		wattron(sur_tasks->win, A_BOLD | COLOR_PAIR(7));
	if (completed_surprise)
		wattron(sur_tasks->win, A_BOLD | COLOR_PAIR(8));

	mvwprintw(sur_tasks->win, 3, (col - strnlen(surprise_task, 256))/2, "%s", surprise_task);

	if (completed_surprise)
		wattron(sur_tasks->win, A_BOLD | COLOR_PAIR(8));
	if (is_surprise)
		wattroff(sur_tasks->win, A_BOLD | COLOR_PAIR(7));

	box(sur_tasks->win, 0, 0);

	//title SURPRISE TASKS
	wattron(sur_tasks->win, A_BOLD | COLOR_PAIR(6));
	title(sur_tasks, "SURPRISE TASKS", 0, col);
	wattroff(sur_tasks->win, A_BOLD | COLOR_PAIR(6));

	wrefresh(sur_tasks->win);

	//defining the player class and setting its attributes
	Player darshan = {0, 0, 0, 0, 0, {0, 0, 0, 0, 0}};

	//all the get methods run python script to fetch data from db
	darshan.level = (int)get_stats("level");

	darshan.coins = get_stats("coins");
	darshan.tasks= (int)get_stats("tasks");
	darshan.streak= (int)get_stats("streak");

	for (int i = 0; i < 5; i++)
	{
		if (i == 4)
			darshan.rating[i] = get_stats("Critical_analysis");
		else
			darshan.rating[i] = get_stats(props[i]);
	}


	disp_info(info, &darshan);

	box(info->win, 0, 0);
	wrefresh(info->win);

	disp_stats(stats, darshan.rating);

	box(stats->win, 0, 0);
	wrefresh(stats->win);

	//displaying streak bars||streak meter on top left corner of TUI
	wattron(streak->win, COLOR_PAIR(3));
	for (int i = 0, j = 0; i < darshan.streak; i++)
	{
		if (i % (col/6 - 2) == 0)
			j++;
		if (j > 10)
			break;
		mvwprintw(streak->win, j, (i)%(col/6 - 2) + 1, "_");
	}
	wattroff(streak->win, COLOR_PAIR(3));

	box(streak->win, 0, 0);
	wrefresh(streak->win);

	//done is array containing whether the task is marked done
	char task[5][256];
	int done[5];

	//fetches all the regular tasks and displays it
	for (int i = 0; i < 5; i++)
	{
		get_task(task[i], 256, i);
		done[i] = get_completed(i);
	}
	disp_tasks(tasks, task, done, 5);

	disp_info(info, &darshan);
	box(info->win, 0, 0);
	wrefresh(info->win);

	box(tasks->win, 0, 0);
	wrefresh(tasks->win);

	//c is the user character input
	int c;
	int x, y;
	//STR is current index of str
	//str is user input string in prompt window
	int STR = 0;
	char str[prompt->width];

	move(prompt->y0 + 2, prompt->x0 + 1);
	wrefresh(prompt->win);

	getyx(stdscr, y, x);
	curs_set(0);
	refresh();

	//insert mode for prompt window similar to vim insert mode
	int insert = 0;

	while (1)
	{
		c = getch();

		switch(c)
		{
			//handling all the mouse clicks
			case KEY_MOUSE:
				if (getmouse(&event) == OK)
				{
					//moving the cursor in promt at mouse location
					if (is_in(prompt, event.y, event.x))
					{
						move(event.y, event.x);
						wrefresh(prompt->win);
					}

					//marking a task by clicking on it and pressing ENTER||RETURN
					//this also calculates the coins gained and number of tasks completed
					if (is_in(tasks, event.y, event.x))
					{
						int task_no = ((event.y - tasks->y0) - 4)/3;

						move(event.y, 1);
						if (getch() == '\n' && done[task_no] != 1)
						{
							done[task_no] ^= 1;
							set_completed(done[task_no], task_no);
							disp_tasks(tasks, task, done, 5);

							if (done[task_no] == 0)
								darshan.curr_tasks--, darshan.tasks--;
							else
								darshan.curr_tasks++, darshan.tasks++;

							darshan.coins++;
							disp_info(info, &darshan);

							box(info->win, 0, 0);
							wrefresh(info->win);

							set_stats("coins", darshan.coins);
							set_stats("tasks", darshan.tasks);
							set_stats("today", darshan.curr_tasks);
						}

						box(tasks->win, 0, 0);
						wrefresh(tasks->win);

						move(prompt->y0 + 2, prompt->x0 + 1);
						wrefresh(prompt->win);

						refresh();
					}

					//handles the surprise task in similar way as tasks
					//but the coins aren't displayed instantly but rather it is added on next day
					if (is_in(sur_tasks, event.y, event.x) && is_surprise)
					{
						if (event.y - sur_tasks->y0 == 3)
						{
							move(event.y, 1);
							if (getch() == '\n')
							{
								completed_surprise ^= 1;
								set_completed(completed_surprise, 5);

								if (completed_surprise)
									wattron(sur_tasks->win, A_BOLD | COLOR_PAIR(8));
								else
									wattron(sur_tasks->win, A_BOLD | COLOR_PAIR(7));

								mvwprintw(sur_tasks->win, 3, (col - strnlen(surprise_task, 256))/2, "%s", surprise_task);

								if (completed_surprise)
									wattroff(sur_tasks->win, A_BOLD | COLOR_PAIR(8));
								else
									wattroff(sur_tasks->win, A_BOLD | COLOR_PAIR(7));
							}
						}
						box(sur_tasks->win, 0, 0);
						wrefresh(sur_tasks->win);

						move(prompt->y0 + 2, prompt->x0 + 1);
						wrefresh(prompt->win);

						refresh();
					}

					//implementing store logic and mouse click on it to buy stuff
					if (is_in(store, event.y, event.x))
					{
						int item_no = ((event.y - store->y0) - 2)/3;
						move(event.y, 1);

						if (getch() == '\n')
						{
							if (store_items[item_no] <= darshan.coins)
								darshan.coins -= store_items[item_no];

							set_stats("coins", darshan.coins);
							disp_info(info, &darshan);

							box(info->win, 0, 0);
							wrefresh(info->win);
						}

						box(store->win, 0, 0);
						wrefresh(store->win);

						move(prompt->y0 + 2, prompt->x0 + 1);
						wrefresh(prompt->win);

						refresh();
					}
				}
				break;

			case 27:
				insert = 0;
				curs_set(0);

				refresh();
				break;

			case ':':
				insert = 1;
				curs_set(1);

				refresh();
				break;

			case 127:
				printw("\b \b");
				STR--;
				break;

			case '\n':
				break;

			//insert mode for user input in prompt
			default:
				getyx(stdscr, y, x);

				if (is_in(prompt, y, x) && insert)
				{
					printw("%c", c);
					str[STR++] = c;
				}
				wrefresh(prompt->win);
		}

		//exiting the game||main window
		if (!insert && c == 'q')
			break;

		//handling the the user input string
		if (c == (int)'\n')
		{
			str[STR++] = '\n';
			str[STR] = '\0';

			FILE *fp = fopen("./db/log.txt", "a+");
			fputs(ctime(&date), fp);
			fputs(str, fp);
			fclose(fp);
			//handle string str
			STR = 0;

			move(prompt->y0 + 2, prompt->x0 + 1);
			box(prompt->win, 0, 0);
			wrefresh(prompt->win);

			getyx(stdscr, y, x);
			curs_set(0);
			insert = 0;
			refresh();
		}
	}

	endwin();

	//freeing all windows and its properties from memory
	free(stats);
	free(tasks);
	free(prompt);
	free(info);
	free(sur_tasks);
	free(store);
	free(streak);
	return 0;
}
