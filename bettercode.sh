#!/bin/sh
METAFILE=~/.bettercode/metadata.txt
CLIENTFILE=~/.bettercode/bettercode.jar
SERVERFILE=~/.bettercode/bettercodeserver.jar
DEPS="jdk11-graalvm-bin"

getDeps() {

}

findFiles() {
	local returnVal=0
	if test -f "$CLIENTFILE"; then
		returnVal=2
	fi
	if test -f "$SERVERFILE"; then
		returnVal=$(($returnVal + 1))
	fi
	return $returnVal
}

install() {
	echo "Running installer"
	curl -s https://api.github.com/repos/vision-05/betterCode/releases
}

update() {
	echo "BetterCode Version $VERSION"
}