from flask import Flask, request, jsonify
import subprocess
import json
from flask_cors import CORS
import logging

from runtime import Runtime
from public import *

app = Flask(__name__)
CORS(app=app, origins=ALLOW_CORS_URLS)
runtime = Runtime(RUNTIME_PATH)
logger = logging.getLogger(__name__)

@app.route("/", methods=["GET"])
def index():
    return "Hello, World!"

@app.route("/api/login", methods=["POST"])
def login():
    data = request.json
    logger.log(logging.INFO, f"On Login(): Received data: {data}")
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
        logger.log(logging.ERROR, e)
        return jsonify({"status": "error", "message": "internal error occurred"})

@app.route("/api/register", methods=["POST"])
def register():
    data = request.json
    logger.log(logging.INFO, f"On Register(): Received data: {data}")
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
        logger.log(logging.ERROR, e)
        return jsonify({"status": "error", "message": "internal error occurred"})

@app.route("/api/active_account_info", methods=["POST", "GET"])
def active_account_info():
    logger.log(logging.INFO, f"On ActiveAccountInfo()")
    if runtime.process is None:
        raise RuntimeError("Runtime is not running.")
    
    try:
        query_text = json.dumps({
            "mode": "active_account_info",
        })
        response = runtime.query(query_text)
        return jsonify(json.loads(response))
    except Exception as e:
        logger.log(logging.ERROR, e)
        return jsonify({"status": "error", "message": "internal error occurred"})


if __name__ == '__main__':
    logging.basicConfig(level=logging.DEBUG)
    runtime.start()
    app.run(port=5000)