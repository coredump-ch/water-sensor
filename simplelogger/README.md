# Simple SSE logger

## Installing

    $ npm install

## Config

Adjust the three `PARTICLE_*` variables in `logger.ts`.

## Running

    $ npm run build
    $ node logger.js

The logger will log all measurements in CSV format to `log.csv`.

## Generating a Particle Cloud API token

See docs at https://docs.particle.io/reference/api/#create-an-oauth-client and
https://docs.particle.io/reference/api/#generate-an-access-token.
Replace all values inside `<angle brackets>` with your own values.

Export your credentials:

    $ export PARTICLE_USERNAME=<joe@example.com>
    $ export PARTICLE_PASSWORD=<your-password>

Get an access token:

    $ curl https://api.particle.io/oauth/token \
        -u particle:particle -d grant_type=password \
        -d username="$PARTICLE_USERNAME" -d password="$PARTICLE_PASSWORD"

Create an OAuth client:

    $ curl https://api.particle.io/v1/clients \
        -d name=<client-name> -d type=installed \
        -d access_token=<access-token>

Get a refresh token using your new client:

    $ curl https://api.particle.io/oauth/token \
        -u <client-id>:<client-secret> -d grant_type=password \
        -d username="$PARTICLE_USERNAME" -d password="$PARTICLE_PASSWORD"

You should now have a client id, a client secret and a refresh token.
