#!/bin/sh
checkDeps() {

}

checkBetterCode() {

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
	curl -s https://api.github.com/repos/vision-05/betterCode/releases/latest | grep "browser_download_url.*tar.gz" | cut -d : -f 2,3 | tr -d \" | wget -qi -
	tar -xvf bettercode.tar.gz #this archive is in the release with installer
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

runClient() {
	client
}

runServer() {
	server &
}

run() {
	runServer
	runClient
}

runHelp() {
	echo "BetterCode options:"
	
	echo "Install program: install"
	echo "Update program: update"
	echo "Uninstall program: uninstall"

	echo "Run client and server: run"
	echo "Run client only: client"
	echo "Run server only: server"
}

parseArgs() {
	if [$1 = "run"] then
		run
	elif [$1 = "client"] then
		runClient
	elif [$1 = "server"] then
		runServer
	elif [$1 = "install"] then
		installDeps
		install
	elif [$1 = "update"] then
		update
	elif [$1 = "uninstall"] then
		uninstall
	elif [$1 = "help"] then
		runHelp
	else
		echo "Invalid command, run bettercode help to see all arguments"
	fi

}

parseArgs