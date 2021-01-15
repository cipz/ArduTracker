FROM php:7.4-apache-buster

# Adding extra packages for php
ADD https://github.com/mlocati/docker-php-extension-installer/releases/latest/download/install-php-extensions /usr/local/bin/
RUN chmod +x /usr/local/bin/install-php-extensions && sync && \
    install-php-extensions mysqli xdebug

# Moving the site
COPY . /var/www/html/