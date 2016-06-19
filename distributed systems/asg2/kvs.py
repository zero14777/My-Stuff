from flask import Flask, request, json, Response, jsonify
import re
app = Flask(__name__)
app.config['MAX_CONTENT_LENGTH'] = 16 * 1024 * 1024

Store = {}

@app.route('/kvs/<key>', methods=['PUT', 'GET', 'DELETE'])
def kvs(key):
    if re.search("[^a-zA-Z0-9_]", key) or len (key) > 251:
        return key
    else:
        if request.method == 'PUT':
            val = request.form.get("val")
            if key in Store.keys():
                Store[key] = val
                data = {
                    'replaced'  : 1,
                    'msg'       : 'success'
                }
                msg = jsonify(data)
                msg.status_code = 200
                return msg
            else:
                Store[key] = val
                data = {
                    'replaced'  : 0,
                    'msg'       : 'success'
                }
                msg = jsonify(data)
                msg.status_code = 201
                return msg
        
        elif request.method == 'GET':
            if key in Store.keys():
                data = {
                    'msg'       : 'success',
                    'value'     : Store[key]
                }
                msg = jsonify(data)
                msg.status_code = 200
                return msg
            else:
                data = {
                    'msg'       : 'error',
                    'error'     : 'key does not exist'
                }
                msg = jsonify(data)
                msg.status_code = 404
                return msg
        
        elif request.method == 'DELETE':
            if key in Store.keys():
                del Store[key]
                data = {
                    'msg' : 'success'
                }
                msg = jsonify(data)
                msg.status_code = 200
                return msg
            else:
                data = {
                    'msg'       : 'error',
                    'error'     : 'key does not exist'
                }
                msg = jsonify(data)
                msg.status_code = 404
                return msg
        
        
if __name__ == '__main__':
    app.run(host ='0.0.0.0', port=8080)
