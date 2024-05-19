# !/bin/bash
# Author: Albertt Itzep
# vamos a crear un comando que nos brinde el contenido de una carpeta en forma de arbol
# con todo lo que se encuentra dentro de ella
# variable generales
yLine="|"
xLine="|--"
xSpace=" "
contX=0
# funcion para el contenido completo de una carpeta
getContentDir() {
	local dir=$1
	local files=$(ls $dir)
	for file in $files; do
		if [[ -d $dir/$file ]]; then
			contX=$((contX + 2))
			echo "Es un directorio $file"
			getContentDir "$dir/$file"
		else
			echo $yLine
			for ((i = 0; i < contX; i++)); do
				echo -n $xSpace
			done
			echo "$xLine No es un directorio $file"
		fi
		if [[ $contX -ge 2 ]]; then
			contx=$((contX - 2))
		fi
	done
}

# validamos que no se haya pasado un argumento  ruta
current_dir=""
if [ $# -eq 0 ]; then
	# Si no se paso un argumento se va a obtener el directorio actual
	current_dir=$PWD
else
	# Si se paso un argumento se va a obtener el directorio que se paso
	current_dir=$1
fi

# Se valida que sea un directorio
if [[ ! -d $current_dir ]]; then
	echo "No es un directorio"
	exit 1
fi

# llamamos a la funcion para obtener el contenido de la carpeta
getContentDir "$current_dir"
