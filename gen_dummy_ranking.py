import os
import sqlite3
import datetime
import random

db_path = r'C:\ProgramData\Typing\ranking.db'

# Ensure directory exists but in this context the DB might exist.
os.makedirs(os.path.dirname(db_path), exist_ok=True)

# Connect to the DB
conn = sqlite3.connect(db_path)
cur = conn.cursor()

# Create table if it doesn't exist (matching C++ definition)
cur.execute('''CREATE TABLE IF NOT EXISTS item(
    id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, 
    category INTEGER NOT NULL, 
    date1 REAL NOT NULL, 
    score1 REAL NOT NULL, 
    typecount1 REAL NOT NULL, 
    misscount1 REAL NOT NULL)''')

# Delete existing dummy data if any to avoid duplication overhead
cur.execute("DELETE FROM item")

# Base date 1899-12-30 for OLE Automation Date
ole_base = datetime.datetime(1899, 12, 30)

start_date = datetime.datetime.now() - datetime.timedelta(days=365 * 5)

category = 1

print("Generating dummy ranking data for 5 years...")
records = []

# Simulate getting better over time
base_score = 1000
base_typecount = 100
base_misscount = 20

current_date = start_date
while current_date <= datetime.datetime.now():
    # Progress factor (0.0 to 1.0) over 5 years
    days_passed = (current_date - start_date).days
    total_days = 365 * 5
    progress = days_passed / total_days

    # Score improves over time: 1000 -> ~6000 + random variance
    score = base_score + (5000 * progress) + random.uniform(-500, 500)
    score = max(0, score)

    # Miss decreases over time: 20 -> ~2 + random variance
    misscount = base_misscount - (18 * progress) + random.uniform(-2, 2)
    misscount = max(0, int(misscount))

    # Typecount goes up with score (roughly)
    typecount = base_typecount + (500 * progress) + random.uniform(-50, 50)
    typecount = max(10, int(typecount))

    delta = current_date - ole_base
    ole_date = delta.days + (delta.seconds / 86400.0)

    records.append((category, ole_date, score, float(typecount), float(misscount)))
    
    # 1 play every 1-3 days
    current_date += datetime.timedelta(days=random.uniform(1, 3))

cur.executemany("INSERT INTO item(category, date1, score1, typecount1, misscount1) VALUES (?, ?, ?, ?, ?)", records)
conn.commit()
conn.close()

print(f"Inserted {len(records)} dummy records successfully.")
