from flask import Flask, request, jsonify
import subprocess
import json
from flask_cors import CORS
import logging

from runtime import Runtime

RUNTIME_PATH = "./builds/gui_runtime"

app = Flask(__name__)
CORS(app=app, origins=['http://localhost:5173'])
runtime = Runtime(RUNTIME_PATH)

@app.route("/", methods=["GET"])
def index():
    return "Hello, World!"

@app.route("/api/login", methods=["POST"])
def login():
    data = request.json
    logging.log(logging.INFO, f"Received data: {data}")
    user_id = data.get('user_id')
    password = data.get('password')
    
    if not user_id or not password:
        raise ValueError("Missing UID or Password")
    if runtime.process is None:
        raise RuntimeError("Runtime is not running.")
    
    try:
        query_text = json.dumps({
            "mode": "login",
            "userid": user_id,
            "password": password,
        })
        response = runtime.query(query_text)
        return jsonify(json.loads(response))
    except Exception as e:
        logging.log(logging.ERROR, e)
        return jsonify({"status": "error", "message": "internal error occurred"})

@app.route("/api/register", methods=["POST"])
def register():
    data = request.json
    logging.log(logging.INFO, f"Received data: {data}")
    user_id = data.get('user_id')
    password = data.get('password')
    username = data.get('username')
    
    if not user_id or not password:
        raise ValueError("Missing UID, Username or Password")
    if runtime.process is None:
        raise RuntimeError("Runtime is not running.")
    
    try:
        query_text = json.dumps({
            "mode": "register",
            "userid": user_id,
            "password": password,
            "username": username,
        })
        response = runtime.query(query_text)
        return jsonify(json.loads(response))
    except Exception as e:
        logging.log(logging.ERROR, e)
        return jsonify({"status": "error", "message": "internal error occurred"})


if __name__ == '__main__':
    # Display the logs on the screen
    logging.basicConfig(level=logging.DEBUG)
    runtime.start()
    app.run(port=5000)