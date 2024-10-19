from datetime import datetime

# Get the current date and time
current_timestamp = datetime.now()

# Format the timestamp
formatted_timestamp = current_timestamp.strftime("%H:%M:%S")
print("Formatted Timestamp:", formatted_timestamp)
