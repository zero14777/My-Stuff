#!/usr/bin/perl

use warnings;
use strict;
use Data::Dumper qw(Dumper);

my $logfile = 'logfile.txt';
open(my $log, ">", $logfile);

system("PdfTE.exe", "input.pdf", "data.txt");

my $bank_summary = "data.txt";
open(my $data, '<', $bank_summary)
	or die "Could not open file '$bank_summary' $!";

my $temp1 = 'temp1.txt';
open(my $fh1, ">", $temp1)
	or die "Could not make file '$temp1' $!";

my $temp2 = 'temp2.txt';
open(my $fh2, ">", $temp2)
	or die "Could not make file '$temp2' $!";

while (my $outerline = <$data>) {
	chomp $outerline;
	
	if($outerline eq "        Date       Description                                                                                 Amount") {
		my $gettingTransaction = 0;
		while (my $line = <$data>) {
			if($line =~ m/^Total.*$/ or $line eq "                                                                                                      continued on the next page") {
				print $fh1 "\n";
				last;
			}
			if ($line =~ m/^        (\d\d\/\d\d\/\d\d)   (.*) *([-]+.*\..*)$/) {
				print $fh2 "$3\n";
				
				if($gettingTransaction) {
					print $fh1 "\n";
				}
				
				my $date = $1;
				my $desc = $2;
				$desc =~ s/[",]//; #" chars would mess up the generated csv file
				print $fh1 "$date,$desc";
				
				$gettingTransaction = 1;
			} elsif (!($line eq "") and $gettingTransaction) {
				print $fh1 "ASDF$line";
			}
		}
	}
}

close($fh1);
close($fh2);
open(my $fh1, "<", $temp1);
open(my $fh2, "<", $temp2);

my $output = 'output.csv';
open(my $fhout, ">", $output);

print $fhout "Date,Description,Amount";

while (1) {
	my $line1 = <$fh1>;
	chomp $line1;
	my $line2 = <$fh2>;
	chomp $line2;
	if ($line1 eq "") {
		if (!($line2 eq "")) {
			print $log __LINE__, ": Too many amounts\n";
		}
		last;
	}
	if ($line2 eq "") {
		print $log __LINE__, ": Too many descriptions\n";
		last;
	}
	print $fhout "\n$line1,$line2"
}