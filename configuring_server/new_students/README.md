# Overview
This guide contains information that may be helpful for new students in the group. Not all sections are essential reading, many Welcome!

# Creating your SSH Key
## Windows
1. Download PuTTy and PuTTygen.
2. Open PuTTygen.
3. Click on `Generate a public/private key pair`.
    1. IMPORTANT: Make sure `RSA` encryption is used.
4. Open PuTTy.
5. Under `Connection->SSH->Auth` make sure the private key you just created is selected.
## Other
1. 


# Install a specific version of Python
## What you need before starting
1. Sudo access to your agent

## Steps
1. SSH to your Agent and open a VNC Server instance.
2. Install the following Python dependencies: `sudo apt install libssl-dev libncurses5-dev libsqlite3-dev libreadline-dev libtk8.6 libgdm-dev libdb4o-cil-dev libpcap-dev`
3. Open a web browser, and from the Python web page select the Python version you want. Download the `XZ compressed source tarball` version.
4. Unzip the Python version using the command `tar -xvf <python>.tar.xz`
5. Follow the instructions in `README.rst` to install Python. Usually, the instructions are to run the following:
    6. ```
       ./configure
       make
       make test
       sudo make install
       ```
   2. NOTE: This will install the Python version as `python3`. If you want to install multiple versions of python (eg. `python3.7`), in the last step run `sudo make altinstall`
   3. if `make test` shows a failure of `test_ssl`, continue with the installation and perform the repair see in [Pip Error: SSL Timeout](https://github.com/UofT-HPRC/tpdp/blob/main/configuring_server/new_students/README.md#pip-error-ssl-timeout)

# Pip Error: SSL Timeout
1. Follow the steps here to edit `openssl.conf`: [https://stackoverflow.com/questions/61568215/openssl-v1-1-1-ubuntu-20-tlsv1-no-protocols-available](https://stackoverflow.com/questions/61568215/openssl-v1-1-1-ubuntu-20-tlsv1-no-protocols-available)
2. Copied here in case link is dead: Make the following changes to `/etc/ssl/openssl.cnf`:
	1. At the top, add: `openssl_conf = default_conf`
 	2. At the bottom, add:
  	3. ```
      	[ default_conf ]
		ssl_conf = ssl_sect
		
		[ssl_sect]
		system_default = system_default_sect
		
		[system_default_sect]
		MinProtocol = TLSv1.2
		CipherString = DEFAULT:@SECLEVEL=1
      	```
