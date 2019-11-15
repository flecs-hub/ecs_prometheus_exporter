# ecs_prometheus_exporter
This example shows how to use the Flecs Prometheus exporter ([flecs-systems-prometheus](https://github.com/flecs-hub/flecs-systems-prometheus)). The Prometheus exporter makes Flecs statistics available as an endpoint that can be scraped by the Prometheus time series database (https://prometheus.io/).

## Prerequisites
Flecs uses the bake build system (https://github.com/SanderMertens/bake). To install bake on Linux and MacOS, do:

```
git clone https://github.com/SanderMertens/bake
make -C bake/build-$(uname)
bake/bake setup
```

You will also need to download and run the [Prometheus](https://prometheus.io/) database if you want to store the data, and [Grafana](https://grafana.com/) if you want to visualize the data. Without these components you can still manually inspect the endpoint.

Once Prometheus is downloaded, copy the `prometheus.yml` file into the directory where Prometheus was unzipped, and run the database by starting the Prometheus process.

When Grafana is started, create a new dashboard, go to configuration (the gear icon) and then "JSON model", and paste the JSON from the `grafana.json` file.

## Running the demo
To install and run the demo, do:

```
bake run flecs-hub/ecs_prometheus_exporter
```

Go to http://localhost:3000 to view the Grafana dashboard. You should see something that looks like this:

<img width="1501" alt="Screen Shot 2019-11-15 at 1 45 58 AM" src="https://user-images.githubusercontent.com/9919222/68933552-be018880-0749-11ea-88cd-54c2c263a64c.png">

