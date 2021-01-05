## ArduTracker Server


### Requirements

- Host OS: `linux / mac os / windows`
- Docker version: `19.x+`
- Docker-compose: `1.8+`
- Arch: `x86 / arm` (64 bit)
- Cpu: `1 core+`
- Ram: `512MB+`
- Optional: `virtualization enabled` (mandatory for windows host os)

### Docker compose

With docker compose, we want to have an auto-configured environment to turn on and off containers without having to configure anything manually in different OS. 
The only 2 commands to use are:

- Start: `docker-compose up --build -d`
- Stop: `docker-compose down -v`

### Mosquitto (MQTT server broker)

To test the mosquitto MQTT server, you can install globally `mqtt` directly from `npm` or any other mqtt client that you prefer.

```
$ npm i -g mqtt

```

#### Test process

Open a terminal (1), change `localhost` to the correct ip / hostname, and send this command

```
$ mqtt sub -h localhost -t matches/#
```

Open another terminal (2), change `localhost` to the correct ip / hostname, and send this command

```
$ mqtt pub -h localhost -t matches/123 -m '{"id": 123, "first_team": {"id": 1, "goals": 1}, "second_team": {"id": 6, "goals": 0}}'
```

