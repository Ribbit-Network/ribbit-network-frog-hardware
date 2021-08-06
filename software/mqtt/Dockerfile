FROM arm32v6/eclipse-mosquitto

# Override v2 default behavior and allow remote access
# also allow anonymous connections
RUN \
    sed -i 's/#listener/listener 1883/' /mosquitto/config/mosquitto.conf && \
    sed -i 's/#allow_anonymous false/allow_anonymous true/' /mosquitto/config/mosquitto.conf
