import sqlite3 as sq
import tasks_gen as tg
from datetime import datetime
import numpy as np

dt = datetime.now()
date = dt.strftime("%x")

conn = sq.connect("./db/tasks.db")
cur = conn.cursor()

stats = sq.connect("./db/stats.db")
curs = stats.cursor()

cur.execute("SELECT date FROM tasks")
data = cur.fetchall()

tasks = []

if len(data) == 0:
    tasks = tg.get_random_tasks()

elif data[-1][0][3:5] != date[3:5]:
    cur.execute("SELECT * FROM tasks")
    query = cur.fetchall()

    curs.execute("SELECT * FROM stats WHERE prop = 'Creativity'")
    creativity = curs.fetchall()[0][-1]
    curs.execute("SELECT * FROM stats WHERE prop = 'Critical_analysis'")
    critical_analysis = curs.fetchall()[0][-1]
    curs.execute("SELECT * FROM stats WHERE prop = 'Agility'")
    agility = curs.fetchall()[0][-1]
    curs.execute("SELECT * FROM stats WHERE prop = 'Strength'")
    strength = curs.fetchall()[0][-1]
    curs.execute("SELECT * FROM stats WHERE prop = 'Academics'")
    academics = curs.fetchall()[0][-1]
    curs.execute("SELECT * FROM stats WHERE prop = 'streak'")
    streak = curs.fetchall()[0][-1]
    curs.execute("SELECT * FROM stats WHERE prop = 'progress'")
    progress = curs.fetchall()[0][-1]
    curs.execute("SELECT * FROM stats WHERE prop = 'level'")
    level = curs.fetchall()[0][-1]

    done = []
    for i in query:
        done.append(i[3])

    agility += 0.08*done[4]/2
    strength += 0.08*done[4]/2
    creativity += 0.06*(done[0] + done[1] + (done[2] or done[3]))/6
    critical_analysis += 0.06*(sum(done)-done[4])/6
    academics += 0.55*(done[2]+done[3])/4

    progress += (1 + streak/10) * 0.25 * np.sqrt((0.35*( creativity/100 )**2 + 0.25*( strength/100 )**2 + 0.15*(agility**2 + critical_analysis**2 + academics**2)/(100**2))/5)
    level = np.floor(progress)

    curs.execute(f"UPDATE stats SET val = {agility} WHERE prop = 'Agility'")
    curs.execute(f"UPDATE stats SET val = {strength} WHERE prop = 'Strength'")
    curs.execute(f"UPDATE stats SET val = {creativity} WHERE prop = 'Creativity'")
    curs.execute(f"UPDATE stats SET val = {critical_analysis} WHERE prop = 'Critical_analysis'")
    curs.execute(f"UPDATE stats SET val = {academics} WHERE prop = 'Academics'")

    curs.execute(f"UPDATE stats SET val = {progress} WHERE prop = 'progress'")
    curs.execute(f"UPDATE stats SET val = {level} WHERE prop = 'level'")

    if len(query) == 6:
        query = query[5][3]

        if query == 1:
            curs.execute("SELECT * FROM stats WHERE prop = 'tasks'")
            no_tasks = curs.fetchall()[0][-1] + 1

            curs.execute("SELECT * FROM stats WHERE prop = 'coins'")
            coins = curs.fetchall()[0][-1] + 3

            curs.execute(f"UPDATE stats SET val = {no_tasks} WHERE prop = 'tasks'")
            curs.execute(f"UPDATE stats SET val = {coins} WHERE prop = 'coins'")
            stats.commit()

    curs.execute("SELECT * FROM stats WHERE prop = 'today'")
    no_tasks = curs.fetchall()[0][-1]

    if  no_tasks >= 5:
        curs.execute("SELECT * FROM stats WHERE prop = 'streak'")
        streak = curs.fetchall()[0][-1] + 1
        curs.execute("SELECT * FROM stats WHERE prop = 'coins'")
        coins = curs.fetchall()[0][-1] + 2

        curs.execute(f"UPDATE stats SET val = {streak} WHERE prop = 'streak'")
        curs.execute(f"UPDATE stats SET val = {coins} WHERE prop = 'coins'")
        stats.commit()
    else:
        curs.execute("SELECT * FROM stats WHERE prop = 'coins'")
        coins = curs.fetchall()[0][-1] - (5 - no_tasks)*0.5

        curs.execute(f"UPDATE stats SET val = {0} WHERE prop = 'streak'")
        curs.execute(f"UPDATE stats SET val = {coins} WHERE prop = 'coins'")
        stats.commit()

    curs.execute(f"UPDATE stats SET val = {0} WHERE prop = 'today'")
    stats.commit()
    tasks = tg.get_random_tasks()
    cur.execute("DELETE FROM tasks")
    conn.commit()

db = []

for i in range(len(tasks)):
    if i < 2:
        db.append((date, tasks[i], 1, 0, 1))
    elif i < 4:
        db.append((date, tasks[i], 2, 0, 0.5))
    elif i < 5:
        db.append((date, tasks[i], 3, 0, 0.5))
    else:
        db.append((date, tasks[i], 0, 0, 2))

cur.executemany("INSERT INTO tasks VALUES (?,?,?,?,?)", db)

conn.commit()
stats.commit()
stats.close()
conn.close()
