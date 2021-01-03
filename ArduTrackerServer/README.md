## ArduTracker Server

#### Docker compose

- Start: `docker-compose up -d`
- Stop: `docker-compose down`

#### Mosquitto test 

```
$ npm i -g mqtt
# mqtt pub -h localhost -t matches/123 -m '{"id": 123, "first_team": {"id": 1, "goals": 1}, "second_team": {"id": 6, "goals": 0}}'
# mqtt sub -h localhost -t matches/#
```