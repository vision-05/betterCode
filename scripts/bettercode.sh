#!/bin/sh
checkDeps() {
	echo "hi"
}

checkBetterCode() {
	echo "hi"
}

installDeps() {
	mkdir "$HOME/bin"
	curl https://raw.githubusercontent.com/technomancy/leiningen/stable/bin/lein > "$HOME/bin/lein"
	chmod a+x "$HOME/bin/lein"
	lein
}


install() {
	echo "Running installer"
	mkdir "$HOME/.bc"
	cd "$HOME/.bc"
	curl -s https://api.github.com/repos/vision-05/betterCode/releases/latest | grep "tarball_url" | cut -d : -f 2,3 | tr -d \" | tr -d ',' > url.txt
	curl -L -o bettercode.tar.gz $(cat url.txt)
	mkdir betterCode
	tar -xvf bettercode.tar.gz -C betterCode --strip-components 1 #this archive is in the release with installer
}

clean() {
	echo "Cleaning current install"
	cd "$HOME/.bc"
	rm -rf bettercode
	rm bettercode.tar.gz
}

update() {
	clean
	install
}

uninstall() {
	rm -rf "$HOME/.bc"
	rm bettercode.sh
	rm client.sh
	rm server.sh
}

client() {
	cd "$HOME/.bc/betterCode/bettercode"
	#this is where ssh port forwarding
	lein run localhost
}

server() {
	cd "$HOME/.bc/betterCode/bettercodeserver"
	lein run
}

run() {
	server &
	client
}

help() {
	echo "BetterCode options:"
	
	echo "Install program: install"
	echo "Update program: update"
	echo "Uninstall program: uninstall"

	echo "Run client and server: run"
	echo "Run client only: client"
	echo "Run server only: server"
}

$1