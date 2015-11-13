from flask import Flask, request, redirect, url_for, g, render_template, jsonify
from dateutil.parser import parse
from datetime import datetime
import json
import urllib2


app = Flask(__name__)


# Basic configuration

PHOTON_ID = '360024001547343433313338'
PARTICLE_ACCESS_TOKEN = 'e32ced60c49dc296d75b3b8c279392c6455254c7'
BASE_URL = 'https://api.particle.io/v1/devices/{}/'.format(PHOTON_ID)


# Helper functions

def photon_url(func):
    return BASE_URL + '{}?access_token={}'.format(PARTICLE_ACCESS_TOKEN)


def call_photon_function(func, args):
    url = photon_url(func)
    r = urllib2.Request(url, json.dumps({'args': args}))
    return urllib2.urlopen(r).read()


def get_photon_var(var):
    url = photon_url(var)
    result = urllib2.urlopen(url).read()
    return json.loads(result['result'])


# Views

@app.route('/')
def index():
    return render_template('coffee.html')


@app.route('/coffee')
def coffee():
    coffee_data = {
        'fill': get_photon_var('fill'),
        'status': get_photon_var('brew_status'),
        'temp': get_photon_var('temp'),
    }
    return jsonify(**coffee_data)


@app.route('/coffee/start')
def start_brew():
    return call_photon_function('brew', 'start')


@app.route('/coffee/stop')
def stop_brew():
    return call_photon_function('brew', 'stop')


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=8888, debug=True)
