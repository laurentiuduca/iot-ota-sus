sudo docker run \
-v "$PWD/jetty.pkcs12:/opt/hawkbit/jetty.pkcs12" \
-p 8443:8443 hawkbit/hawkbit-update-server:latest \
--security.user.name=admin --security.user.password=admin --spring.main.allow-bean-definition-overriding=true \
--hawkbit.server.ddi.security.authentication.targettoken.enabled=true \
--hawkbit.server.repository.publish-target-poll-event=false \
--hawkbit.controller.pollingTime=00:00:30 \
--hawkbit.dmf.rabbitmq.enabled=false \
--hawkbit.artifact.url.protocols.download-http.protocol=https \
--hawkbit.artifact.url.protocols.download-http.port=8443 \
--security.require-ssl=true \
--server.use-forward-headers=true \
--server.port=8443 \
--server.ssl.key-store=jetty.pkcs12 \
--server.ssl.key-store-password=admin12 \
--server.ssl.key-password=admin12 \
--server.ssl.protocol=TLS \
--server.ssl.enabled-protocols=TLSv1,TLSv1.1,TLSv1.2,TLSv1.3 \
--hawkbit.server.ddi.security.authentication.anonymous.enabled=true
# --hawkbit.server.ddi.security.authentication.anonymous.enabled=true \

