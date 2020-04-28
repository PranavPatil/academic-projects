#ifndef _server_h
#define _server_h

#define KEYFILE "cert/server.pem"
#define PASSWORD "password"
#define DHFILE "cert/dh1024.pem"

int tcp_listen(void);
void load_dh_params(SSL_CTX *ctx,char *file);
void generate_eph_rsa_key(SSL_CTX *ctx);

#endif

