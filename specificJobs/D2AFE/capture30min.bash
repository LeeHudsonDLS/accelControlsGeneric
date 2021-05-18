for i in {1..450}; do echo 1 mon$'\r'; sleep 1;echo 2 mon$'\r'; sleep 1;echo 3 mon$'\r'; sleep 1;echo 4 mon$'\r'; sleep 1; done | nc ts0011.diamond.ac.uk 7015 >capture30min.csv
