cd ./dataset/cta_dataset/train/;
tar -czf data.tar.gz data*.csv;
mv data.tar.gz ../../../../RuTaBERT/data/train/;

cd ../test/;
tar -czf data.tar.gz data.csv;
mv data.tar.gz ../../../../RuTaBERT/data/test/;

cd ../../../../RuTaBERT/data/train/;
tar -xvf data.tar.gz;

cd ../test/
tar -xvf data.tar.gz;

