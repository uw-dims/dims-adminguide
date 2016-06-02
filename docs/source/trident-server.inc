

  # Our webroot (contains static, non-sensitive files, source if public ;)
  root /usr/share/trident/webroot/;

  ######################################################
  # Static files
  ######################################################
  location /css/ {
  }

  location /favicon.ico {
  }

  location /gfx/ {
  }

  location /js/ {
  }

  ######################################################
  # Forward all requests to the Trident Daemon
  ######################################################
  location / {
    client_max_body_size    0;
    proxy_set_header  Host $host;
    proxy_http_version  1.1;
    proxy_pass    http://trident-daemon;
  }

