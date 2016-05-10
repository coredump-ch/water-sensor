import json
import logging
import os
import sys

import requests
from sseclient import SSEClient

# Helper functions

env = os.environ.get


def require_env(name):
    value = env(name)
    if not value:
        raise ValueError('Missing %s env variable' % name)
    return value


logger = logging.getLogger('main')
logger_particle = logging.getLogger('particle')

PARTICLE_CLIENT_ID = require_env('PARTICLE_CLIENT_ID')
PARTICLE_CLIENT_SECRET = require_env('PARTICLE_CLIENT_SECRET')
PARTICLE_REFRESH_TOKEN = require_env('PARTICLE_REFRESH_TOKEN')


def get_access_token():
    """
    Get a valid access token.

    TODO: Cache access token.

    """
    # Prepare request
    url = 'https://api.particle.io/oauth/token'
    auth = (PARTICLE_CLIENT_ID, PARTICLE_CLIENT_SECRET)
    data = {'grant_type': 'refresh_token', 'refresh_token': PARTICLE_REFRESH_TOKEN}

    # Send request
    logger_particle.info('Fetching access token...')
    response = requests.post(url, auth=auth, data=data)
    if response.status_code != 200:
        logger_particle.error('Could not retrieve access token (HTTP %s)', response.status_code)
        logger_particle.error('Are your refresh token and client credentials correct?')
        sys.exit(1)

    # Decode response
    data = response.json()
    return data['access_token']


def process_event(msg):
    """
    Process an event message from the cloud.
    """
    print(msg)


def listen_for_events(access_token):
    """
    Wait for events. Blocks until interrupted.
    """
    headers = {'Authorization': 'Bearer %s' % access_token}
    logger_particle.info('Listening for events...')
    url = 'https://api.particle.io/v1/devices/events?access_token=' + access_token
    messages = SSEClient(url, retry=1000 * 5)
    for msg in messages:
        logger_particle.info('New event received')
        process_event(msg)


# Entry point

if __name__ == '__main__':

    # Set up logging
    logging.basicConfig(level=logging.DEBUG)

    # For each device, fetch and upload the variables
    access_token = get_access_token()
    logger.debug('Access token is %s', access_token)

    # Wait for events
    listen_for_events(access_token)
