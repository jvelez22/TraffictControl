import serial
import json
import firebase_admin
from firebase_admin import credentials, firestore
from firebase_admin.firestore import SERVER_TIMESTAMP

# Initialize Firebase Admin SDK
cred = credentials.Certificate("/Users/jvelez/Downloads/trafficsystemdata-firebase-adminsdk-bsvc7-43ff84c64d.json")  # Path to your Firebase service account key
firebase_admin.initialize_app(cred)

# Reference to your database
db = firestore.client()

# Reference to a Firestore collection
collection_ref = db.collection('data')  # Replace 'data' with your desired collection name

# Open Serial Connection
ser = serial.Serial('/dev/cu.usbmodem143301', 115200)  # Replace with your COM port and baud rate
print("Connected to Arduino")

# Continuously read data
while True:
    try:
        # Read and decode serial data
        line = ser.readline().decode('utf-8').strip()
        print(f"Received: {line}")
        
        # Parse the data if it's JSON
        data = json.loads(line)  # Convert JSON string to Python dictionary
        data["createdAt"] = SERVER_TIMESTAMP
        
        # Add the data to Firestore as a new document
        collection_ref.add(data)  # Automatically generates a unique document ID
        print("Data sent to Firestore")
    except json.JSONDecodeError:
        print("Invalid JSON data received")
    except KeyboardInterrupt:
        print("Stopping...")
        ser.close()
        break
