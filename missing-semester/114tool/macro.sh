
VAR_MACRO="$HOME/tmp_macro"

macro () {
	touch $VAR_MACRO
	echo "$(pwd)" > $VAR_MACRO
}

polo () {
	echo "running polo() in macro.sh"
	cd "$(cat $VAR_MACRO)"
}
