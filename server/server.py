from flask import Flask
import argparse

app = Flask(__name__)
@app.route("/")

def index():
    return "Hello HTML"

if __name__ == "__main__":
    parser = argparse.ArgumentParser(prog="server")
    parser.add_argument('--debug', action='store_true', required=False, help='Enable debug mode')
    parser.add_argument('--port', type=int, default=5000, required=False, help='Port number (default: 5000)')
    args = parser.parse_args()
    args = parser.parse_args()
    print('Hello,', args.debug)
    app.run(port=args.port,debug=args.debug)
    