import os
import sqlite3 as sq

if os.path.isfile("./db/stats.db"):
    os.remove("./db/stats.db")

if os.path.isfile("./db/tasks.db"):
    os.remove("./db/tasks.db")

conn = sq.connect("./db/stats.db")

cursor = conn.cursor()

cursor.execute("""
    CREATE TABLE stats
    (
        prop text,
        val real
    )
""")
conn.commit()

initial_stats = [
    ("level", 1),
    ("progress", 1),
    ("coins", 0),
    ("tasks", 0),
    ("streak", 0),
    ("Agility", 7.5),
    ("Strength", 2.5),
    ("Academics", 4),
    ("Creativity", 4),
    ("Critical_analysis", 6),
    ("today", 0)
]
cursor.executemany("INSERT INTO stats VALUES (?,?)", initial_stats)
conn.commit()

conn.close()

conn = sq.connect("./db/tasks.db")

cursor = conn.cursor()

cursor.execute("""
    CREATE TABLE tasks
    (
        date text,
        task text,
        type integer,
        done integer,
        coin real
    )
""")
conn.commit()
conn.close()

os.system("python3 ./scripts/tasks.py")
