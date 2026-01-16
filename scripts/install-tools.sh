#!/usr/bin/env bash

sudo apt update

# verilator prerequisites
sudo apt-get install -y git help2man perl python3 make autoconf g++ flex bison ccache
sudo apt-get install -y libgoogle-perftools-dev numactl perl-doc
sudo apt-get install -y libfl2  # Ubuntu only (ignore if gives error)
sudo apt-get install -y libfl-dev  # Ubuntu only (ignore if gives error)
sudo apt-get install -y zlibc zlib1g zlib1g-dev  # Ubuntu only (ignore if gives error)

unset VERILATOR_ROOT  # For bash

sudo apt-get install -y gtkwave

# JDK
# Ensure the necessary packages are present:
sudo apt install -y wget gpg apt-transport-https
# Download the Eclipse Adoptium GPG key:
wget -qO - https://packages.adoptium.net/artifactory/api/gpg/key/public | gpg --dearmor | sudo tee /etc/apt/trusted.gpg.d/adoptium.gpg > /dev/null
# Configure the Eclipse Adoptium apt repository
echo "deb https://packages.adoptium.net/artifactory/deb $(awk -F= '/^VERSION_CODENAME/{print$2}' /etc/os-release) main" | sudo tee /etc/apt/sources.list.d/adoptium.list
# Install
sudo apt install temurin-17-jdk

# Install Scala with cs setup (recommended)
# on the x86_64
curl -fL https://github.com/coursier/coursier/releases/latest/download/cs-x86_64-pc-linux.gz | gzip -d > cs && chmod +x cs && ./cs setup


# We recommend Chisel users use Mill.
#curl -L https://raw.githubusercontent.com/lefou/millw/0.4.11/millw > mill && chmod +x mill
curl -L https://repo1.maven.org/maven2/com/lihaoyi/mill-dist/1.1.0-RC3/mill-dist-1.1.0-RC3-mill.sh -o mill
chmod +x mill
sudo mv mill /usr/local/bin/

mill mill.tabcomplete/install
#echo '*.mill linguist-language=Scala' > .gitattributes

