///<reference path="types/request-promise.d.ts" />

'use strict';

let EventSource = require('eventsource');
let request = require('request-promise');
let json = require('json-promise');
var fs = require('fs');

let PARTICLE_CLIENT_ID = 'changeme'
let PARTICLE_CLIENT_SECRET = 'changeme'
let PARTICLE_REFRESH_TOKEN = 'changeme'


/**
 * Return a particle access token.
 */
async function getAccessToken(): Promise<string> {
    // Request options
    let options = {
        method: 'POST',
        uri: 'https://api.particle.io/oauth/token',
        auth: {
            'user': PARTICLE_CLIENT_ID,
            'pass': PARTICLE_CLIENT_SECRET,
        },
        form: {
            'grant_type': 'refresh_token',
            'refresh_token': PARTICLE_REFRESH_TOKEN,
        }
    };

    // Fetch token
    console.info('Fetching access token...')
    let body = await request(options).catch((err) => {
        console.error('Could not retrieve access token (HTTP ' + err.response.statusCode + ')')
        console.error('Are your refresh token and client credentials correct?')
        throw err;
    });

    // Parse response
    let data = await json.parse(body);
    return data.access_token;
}

/**
 * Process a status change event.
 */
function processStatus(event: any) {
    let data = JSON.parse(event.data);
    console.info(data.published_at, 'Status update:', data.data);
}

/**
 * Process a measurement event.
 */
function processMeasurement(event: any) {
    let data = JSON.parse(event.data);
    console.info(data.published_at, 'Measurement:', data.data);
    fs.appendFile('log.csv', data.published_at + ',' + data.data + '\n');
}

async function main() {
    let token = await getAccessToken();
    let url = 'https://api.particle.io/v1/devices/events?access_token=' + token;
    console.info('Connecting to', url);
    let es = new EventSource(url);
    es.addEventListener('spark/status', processStatus);
    es.addEventListener('measurement', processMeasurement);
}

main().then(() => console.info('Waiting for events...'));
