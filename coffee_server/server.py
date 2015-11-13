from flask import Flask, request, redirect, url_for, g, render_template
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
    return BASE_URL + str(func)


def call_photon_function(func, args):
    url = photon_post_url(func)
    r = urllib2.Request( \
        url, \
        {'access_token': PARTICLE_ACCESS_TOKEN, 'args': args}, \
        {"Content-type": "application/json"} \
    )
    return urllib2.urlopen(r).read()


def get_photon_var(var):
    url = photon_url(var) + '?access_token={}'.format(func, PARTICLE_ACCESS_TOKEN)
    return urllib2.urlopen(url).read()


# Views

@app.route('/')
def index():
    return render_template('coffee.html')


@app.route('/coffee', methods=['GET', 'POST'])
def coffee():
    if request.method == 'POST':
        cache = {
            'brew': request.form['brew'],
            'time': datetime.now().isoformat(),
        }
        with open('cache', 'r') as f:
            f.write(json.dumps(cache))
        call_photon_function('control_brew', 'start')
        return redirect(url_for('coffee'))

    with open('cache', 'r') as f:
        data = f.read()
    data = json.loads(data)
    time = parse(data['time'])
    coffee_data = {
        'brew': data['brew'],
        'fill': '80',
        'status': get_photon_var('brew_status'),
        'temp': get_photon_var('temp'),
        'time': time,
    }
    return coffee_data


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=8888, debug=True)
