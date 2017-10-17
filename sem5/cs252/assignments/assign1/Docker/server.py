import flask
app = Flask(__name__)

@app.route("/")
def home():
	return "<h1>Hello World</h1>"
