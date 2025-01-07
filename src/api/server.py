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
    userid = data.get('userid')
    password = data.get('password')
    
    if not userid or not password:
        raise ValueError("Missing UID or Password")
    if runtime.process is None:
        raise RuntimeError("Runtime is not running.")
    
    try:
        query_text = json.dumps({
            "mode": "login",
            "userid": userid,
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
    userid = data.get('userid')
    password = data.get('password')
    username = data.get('username')
    
    if not userid or not password:
        raise ValueError("Missing UID, Username or Password")
    if runtime.process is None:
        raise RuntimeError("Runtime is not running.")
    
    try:
        query_text = json.dumps({
            "mode": "register",
            "userid": userid,
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

@app.route("/api/find_books", methods=["POST"])
def find_books():
    data = request.json
    logger.log(logging.INFO, f"On FindBooks(): Received data: {data}")
    if runtime.process is None:
        raise RuntimeError("Runtime is not running.")
    query_mode = data.get('query_mode')
    query_content = data.get('query_content')
    query_keywords: list[str] = data.get('query_tags')
    query_keywords: str = '|'.join(query_keywords)
    try:
        logger.log(logging.INFO, f"Query Mode: {query_mode}")
        query_text = json.dumps({
            "mode": "find_books",
            "query_type": query_mode,
            "content": query_keywords if query_mode == "keywords" else query_content,
        })
        response = runtime.query(query_text)
        # turn the keywords in the response into lists
        response = json.loads(response)
        if (response["status"] == "success"):
            for book in response["content"]:
                book["keyword"] = book["keyword"].split("|")
        logger.log(logging.INFO, f"Edited Response: {response}")
        return jsonify(response)
    except Exception as e:
        logger.log(logging.ERROR, e)
        return jsonify({"status": "error", "message": "internal error occurred"})

@app.route("/api/buy_book", methods=["POST"])
def buy_book():
    data = request.json
    logger.log(logging.INFO, f"On BuyBook(): Received data: {data}")
    if runtime.process is None:
        raise RuntimeError("Runtime is not running.")
    book_isbn = data.get('isbn')
    buy_amount = data.get('amount')
    try:
        query_text = json.dumps({
            "mode": "buy_book",
            "isbn": book_isbn,
            "amount": buy_amount,
        })
        response = runtime.query(query_text)
        return jsonify(json.loads(response))
    except Exception as e:
        logger.log(logging.ERROR, e)
        return jsonify({"status": "error", "message": "internal error occurred"})

@app.route("/api/import_book", methods=["POST"])
def import_book():
    data = request.json
    logger.log(logging.INFO, f"On ImportBook(): Received data: {data}")
    if runtime.process is None:
        raise RuntimeError("Runtime is not running.")
    book_isbn = data.get('isbn')
    book_amount = data.get('amount')
    import_cost = data.get('cost')
    # Convert the amount to integer; if it fails, return an error
    try:
        book_amount = int(book_amount)
        assert book_amount > 0
    except (ValueError, AssertionError):
        return jsonify({"status": "error", "message": "amount must be an integer"})
    # Convert import_cost to a float; if it fails, return an error
    try:
        import_cost = float(import_cost)
        assert import_cost > 0
    except (ValueError, AssertionError):
        return jsonify({"status": "error", "message": "cost must be a positive float"})
    # If all conversions are successful, proceed with the query
    try:
        query_text = json.dumps({
            "mode": "import_book",
            "isbn": book_isbn,
            "amount": book_amount,
            "cost": import_cost,
        })
        response = runtime.query(query_text)
        return jsonify(json.loads(response))
    except Exception as e:
        logger.log(logging.ERROR, e)
        return jsonify({"status": "error", "message": "internal error occurred"})

@app.route("/api/modify", methods=["POST"])
def modify():
    data = request.json
    logger.log(logging.INFO, f"On Modify(): Received data: {data}")
    if runtime.process is None:
        raise RuntimeError("Runtime is not running.")
    book_isbn = data.get('isbn')
    original_isbn = data.get('original_isbn')
    book_title = data.get('title')
    book_author = data.get('author')
    book_keywords = data.get('keywords')    # is a list here
    book_keywords = '|'.join(book_keywords)
    book_price = data.get('price')          # is a string here
    on_collision = data.get('on_collision') # ['abort', 'warn', 'ignore']
    # Assert that all required fields are present
    if not book_isbn or not book_title or not book_author or not book_price:
        return jsonify({"status": "error", "message": "Missing required fields"})
    # Convert the price to a float; if it fails, return an error
    try:
        book_price = float(book_price)
        assert book_price > 0
    except (ValueError, AssertionError):
        return jsonify({"status": "error", "message": "Price must be a positive float"})
    # If all conversions are successful, proceed with the query
    isbn_occupied = False
    try:
        query_text = json.dumps({
            "mode": "check_isbn",
            "isbn": book_isbn,
        })
        response = runtime.query(query_text)
        isbn_occupied = json.loads(response)["content"]
    except Exception as e:
        logger.log(logging.ERROR, e)
        return jsonify({"status": "error", "message": "internal error occurred"})
    if isbn_occupied and on_collision == "abort":
        return jsonify({"status": "error", "message": "ISBN is already in use"})
    try:
        # First select
        query_text = json.dumps({
            "mode": "select",
            "isbn": original_isbn,
        })
        response = runtime.query(query_text)
        logger.log(logging.INFO, f"Select Response: {response}")
        # Then modify
        query_text = json.dumps({
            "mode": "modify",
            "isbn": book_isbn,
            "title": book_title,
            "author": book_author,
            "keyword": book_keywords,
            "price": book_price,
        })
        response = runtime.query(query_text)
        return jsonify(json.loads(response))
    except Exception as e:
        logger.log(logging.ERROR, e)
        return jsonify({"status": "error", "message": "internal error occurred"})

@app.route("/api/logout", methods=["POST"])
def logout():
    logger.log(logging.INFO, f"On Logout()")
    if runtime.process is None:
        raise RuntimeError("Runtime is not running.")
    
    try:
        query_text = json.dumps({
            "mode": "logout",
        })
        response = runtime.query(query_text)
        return jsonify(json.loads(response))
    except Exception as e:
        logger.log(logging.ERROR, e)
        return jsonify({"status": "error", "message": "internal error occurred"})

@app.route("/api/get_transactions", methods=["POST"])
def get_transactions():
    logger.log(logging.INFO, f"On GetTransactions()")
    if runtime.process is None:
        raise RuntimeError("Runtime is not running.")
    
    try:
        query_text = json.dumps({
            "mode": "get_transactions",
        })
        response = runtime.query(query_text)
        return jsonify(json.loads(response))
    except Exception as e:
        logger.log(logging.ERROR, e)
        return jsonify({"status": "error", "message": "internal error occurred"})

@app.route("/api/get_worker_report", methods=["POST"])
def get_worker_report():
    logger.log(logging.INFO, f"On GetWorkerReport()")
    if runtime.process is None:
        raise RuntimeError("Runtime is not running.")
    
    try:
        query_text = json.dumps({
            "mode": "get_worker_report",
        })
        response = runtime.query(query_text)
        return jsonify(json.loads(response))
    except Exception as e:
        logger.log(logging.ERROR, e)
        return jsonify({"status": "error", "message": "internal error occurred"})

@app.route("/api/get_logs", methods=["POST"])
def get_logs():
    logger.log(logging.INFO, f"On GetLogs()")
    if runtime.process is None:
        raise RuntimeError("Runtime is not running.")
    
    try:
        query_text = json.dumps({
            "mode": "get_logs",
        })
        response = runtime.query(query_text)
        return jsonify(json.loads(response))
    except Exception as e:
        logger.log(logging.ERROR, e)
        return jsonify({"status": "error", "message": "internal error occurred"})

@app.route("/api/get_all_accounts", methods=["POST"])
def get_all_accounts():
    logger.log(logging.INFO, f"On GetAllAccounts()")
    if runtime.process is None:
        raise RuntimeError("Runtime is not running.")
    
    try:
        query_text = json.dumps({
            "mode": "get_all_accounts",
        })
        response = runtime.query(query_text)
        return jsonify(json.loads(response))
    except Exception as e:
        logger.log(logging.ERROR, e)
        return jsonify({"status": "error", "message": "internal error occurred"})

@app.route("/api/get_login_stack", methods=["POST"])
def get_login_stack():
    logger.log(logging.INFO, f"On GetLoginStack()")
    if runtime.process is None:
        raise RuntimeError("Runtime is not running.")
    
    try:
        query_text = json.dumps({
            "mode": "get_login_stack",
        })
        response = runtime.query(query_text)
        return jsonify(json.loads(response))
    except Exception as e:
        logger.log(logging.ERROR, e)
        return jsonify({"status": "error", "message": "internal error occurred"})

@app.route("/api/password", methods=["POST"])
def password():
    data = request.json
    logger.log(logging.INFO, f"On Password(): Received data: {data}")
    if runtime.process is None:
        raise RuntimeError("Runtime is not running.")
    userid = data.get('userid')
    old_password = data.get('old_password')
    new_password = data.get('new_password')
    try:
        query_text = json.dumps({
            "mode": "password",
            "userid": userid,
            "old_password": old_password,
            "new_password": new_password,
        })
        response = runtime.query(query_text)
        return jsonify(json.loads(response))
    except Exception as e:
        logger.log(logging.ERROR, e)
        return jsonify({"status": "error", "message": "internal error occurred"})

@app.route("/api/add_user", methods=["POST"])
def add_user():
    data = request.json
    logger.log(logging.INFO, f"On AddUser(): Received data: {data}")
    if runtime.process is None:
        raise RuntimeError("Runtime is not running.")
    userid = data.get('userid')
    password = data.get('password')
    username = data.get('username')
    privilege = data.get('privilege')
    try:
        query_text = json.dumps({
            "mode": "add_user",
            "userid": userid,
            "password": password,
            "username": username,
            "privilege": str(privilege),
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