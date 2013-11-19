umount /mnt/usb

find /home/root/mainpr -type f -name "SourceData*" -delete
rm -f sourcefile

DIR=/mnt/usb/Mail.box

for i in `ls /dev/sd*`
do
        echo $i
	mount $i /mnt/usb
done

if [ ! -d "$DIR" ]
then
    echo "-1"
fi

source_data_file=`find $DIR -regex "$DIR/SourceData-[0-9]*\.\(xml\|Xml\|XML\)"`
echo $source_data_file
if [ $source_data_file ]
then
    echo $source_data_file
    cp $source_data_file /home/root/mainpr
    ls SourceData* > sourcefile
    exit
fi

source_data_file=`find $DIR -regex "$DIR/SourceData-[0-9]*\.\(bin\|BIN\|Bin\)"`
echo $source_data_file
if [ $source_data_file ]
then
    unzip $source_data_file 
    ls SourceData* > sourcefile
    exit
fi

echo "-2" > sourcefile
