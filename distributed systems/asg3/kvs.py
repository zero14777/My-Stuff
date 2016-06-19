from flask import Flask, request, json, Response, jsonify
import threading
import json
import requests
import time
import re
import os # for env variables

app = Flask(__name__)
app.config['MAX_CONTENT_LENGTH'] = 16 * 1024 * 1024

ip_env = os.environ['IP']
port_env = os.environ['PORT']
members_env = os.environ['MEMBERS'].split(",") # array w/ elements ip:port


# ranks from 0 to 4, this is your actual rank
self_rank = int(ip_env[-1:])
# this is rank of your leader
assoc_rank = self_rank
Store = {}
version = 0
leader = {'ip': ip_env, 'version' : version, 'self_rank' : self_rank,
          'assoc_rank' : assoc_rank}
self_leader = leader

@app.route('/node/<key>', methods=['PUT', 'DELETE'])
def node(key):
    global Store
    if request.method == 'PUT':
        val = request.form.get("val")
        timey = request.form.get("time")
        Store[key] = [val, timey]
        return 'success'

    if request.method == 'DELETE':
        timey = request.form.get("time")
        Store[key] = [0, timey]
        return 'success'
        
    

@app.route('/node/ping', methods=['GET'])
def ping():
    data = {
        'ip'        : ip_env,
        'port'      : port_env,
        'version'   : version,
        'self_rank' : self_rank,
        'assoc_rank': assoc_rank
    }
    msg = jsonify(data)
    msg.status_code = 200
    return msg


@app.route('/node/update', methods=['GET'])
def update():
    msg = jsonify(Store)
    msg.status_code = 200
    return msg

# receives a dict from other node and merges it with local dict
@app.route('/node/merge', methods=['PUT'])
def merge():
    global version
    version_inc = 0
    
    #if you are not leader then throw request up
    if ip_env != leader['ip']:
        try:
            res = requests.put('http://'+ i + '/node/merge',
                               json = json.dumps(Store), timeout=30  )
            return res
        except (requests.ConnectionError, requests.Timeout):
            pass
        return "merge failed"

        
    foreign_dict = json.loads(request.get_json())
    for key, value in foreign_dict.items():
        # already in, need to do timestamp stuff
        if key in Store:
            Store[key] = value
            version_inc = 1
        else:
            Store[key] = value
            version_inc = 1

    version = version + version_inc
    return "merged"

@app.route('/kvs/<key>', methods=['PUT', 'GET', 'DELETE'])
def kvs(key):
    if re.search("[^a-zA-Z0-9_]", key) or len (key) > 250:
        data = { 'msg' : 'error'}
        msg = jsonify(data)
        msg.status_code = 400
        return msg
    else:
        if request.method == 'PUT':
            global version
            global leader
            global self_leader
            val = request.form.get("val")
            if val == None:
                data = {'msg' : 'error'}
                msg = jsonify(data)
                msg.status_code = 400
                return msg
            if leader['ip'] != self_leader['ip']:
                try:
                    dest = 'http://' + leader['ip'] + ':' + leader['port'] + '/kvs/' + key
                    res = requests.put(dest, data = {'val':val}, timeout=30  )

                    data = res.json()
                    if (data['msg'] == 'error'):
                        data = {
                            'msg'       : 'error',
                            'error'     : 'Connection to leader failed. Try sending again in a few seconds.'
                        }
                        msg = jsonify(data)
                        msg.status_code = 408
                        return msg
                except (requests.ConnectionError, requests.Timeout):
                    data = {
                        'msg'       : 'error',
                        'error'     : 'Connection to leader failed. Try sending again in a few seconds.'
                    }
                    msg = jsonify(data)
                    msg.status_code = 408
                    return msg

            # for the leader
            if key in Store.keys():
                if Store[key][0] != 0:
                    timey = time.time()
                    Store[key] = [val, timey]
                    version = version + 1
                    data = {
                        'replaced'  : 1,
                        'msg'       : 'success'
                    }
                    msg = jsonify(data)
                    msg.status_code = 200

                    # call node/<key> with put on all nodes not self
                    for i in members_env:
                        if i == ip_env:
                            continue
                        try:
                            res = requests.put('http://'+ i + '/node/'
                                               + key, data = {'val':val,
                                                              'time':timey},
                                               timeout=.5 )
                        except (requests.ConnectionError, requests.Timeout):
                            pass
                    return msg
            timey =  time.time()
            Store[key] = [val, timey]
            version = version + 1
            data = {
                'replaced'  : 0,
                'msg'       : 'success'
            }
            msg = jsonify(data)
            msg.status_code = 201
            # call node/<key> with put on all nodes not self
            for i in members_env:
                if i == ip_env:
                    continue
                try:
                    res = requests.put('http://'+ i + '/node/'
                                       + key, data = {'val':val,
                                                      'time':timey},
                                       timeout=.5 )
                except (requests.ConnectionError, requests.Timeout):
                    pass
            return msg
            
        elif request.method == 'GET':
            if key in Store.keys():
                if Store[key][0] != 0:
                    data = {
                        'msg'       : 'success',
                        'value'     : Store[key][0]
                    }
                    msg = jsonify(data)
                    msg.status_code = 200
                    return msg
            
            data = {
                'msg'       : 'error',
                'error'     : 'key does not exist'
            }
            msg = jsonify(data)
            msg.status_code = 404
            return msg
        
        elif request.method == 'DELETE':
            global version
            global leader
            global self_leader
            if leader['ip'] != self_leader['ip']:
                try:
                    dest = 'http://' + leader['ip'] + ':' + leader['port'] + '/kvs/' + key
                    res = requests.delete(dest, timeout=30  )
                    data = res.json()
                    if (data['msg'] == 'error'):
                        data = {
                            'msg'       : 'error',
                            'error'     : data['error']
                        }
                        msg = jsonify(data)
                        msg.status_code = res.status_code
                        return msg
                except (requests.ConnectionError, requests.Timeout):
                    data = {
                        'msg'       : 'error',
                        'error'     : 'Connection to leader failed. Try sending again in a few seconds.'
                    }
                    msg = jsonify(data)
                    msg.status_code = 408
                    return msg

            if key in Store.keys():
                if Store[key][0] != 0:
                    timey = time.time()
                    Store[key] = [0, timey]
                    version = version + 1
                    data = {
                        'msg' : 'success'
                    }
                    msg = jsonify(data)
                    msg.status_code = 200

                    for i in members_env:
                        if i == ip_env:
                            continue
                        try:
                            res = requests.delete('http://'+ i + '/node/'
                                               + key, data = {'time':timey},
                                               timeout=.5 )
                        except (requests.ConnectionError, requests.Timeout):
                            pass
                    return msg
            data = {
                'msg'       : 'error',
                'error'     : 'key does not exist'
            }
            msg = jsonify(data)
            msg.status_code = 404
            return msg

def heartbeat():
    global leader
    global Store
    global version

    potential_leader = leader
    old_leader_alive = False

    # there might be a subtle problem here
    
    for i in reversed(members_env):
        if i == ip_env:
            continue
        try:
            res = requests.get('http://'+ i + '/node/ping', timeout=10  )
            data = res.json()
            if data['ip'] == leader['ip']:
                old_leader_alive = True
            if (data['assoc_rank'] > potential_leader['assoc_rank']
                or data['self_rank'] >= potential_leader['assoc_rank']): 
                potential_leader = data
        except (requests.ConnectionError, requests.Timeout):
            pass

    #if the old leader died, set yourself to be your own leader
    #and call heartbeat again
    
    if not old_leader_alive:
        self_leader['version'] = version
        leader = self_leader
        threading.Timer(2.0, heartbeat).start()
        return
    #its not just the same leader, so its a new leader
    elif potential_leader['ip'] != leader['ip']:
        leader = potential_leader
        # pass dict to new leader merge request
        try:
            if ip_env != leader['ip']:
                res = requests.put('http://'+ leader['ip'] + ':' + leader['port']
                                   + '/node/merge',
                                   json = json.dumps(Store), timeout=30  )
        except (requests.ConnectionError, requests.Timeout):
            pass
    #leader is same, but version changed
    elif potential_leader['version'] != leader['version']:
        leader = potential_leader
        #get whole table from leader update request
        try:
            if ip_env != leader['ip']:

                res = requests.get('http://'+ leader['ip']+ ':' + leader['port']
                                   + '/node/update',
                                   timeout=30 )
                Store = res.json()
                version = version + 1
        except (requests.ConnectionError, requests.Timeout):
            pass
        
    threading.Timer(2.0, heartbeat).start()
        
if __name__ == '__main__':
    heartbeat()
    app.run(host=ip_env, port=int(port_env))
