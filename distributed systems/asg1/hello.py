from flask import Flask, request
app = Flask(__name__)

@app.route('/hello', methods=['GET'])
def hello():
    return "Hello World!\n"

@app.route('/echo', methods=['GET'])
def echo():
    if 'msg' in request.args:
        return '' + request.args['msg'] + '\n'
    else:
        return ''

if __name__ == '__main__':
    app.run(host ='0.0.0.0', port=8080)
