#!/usr/bin/perl

use warnings;
use strict;
use Data::Dumper qw(Dumper);

my $logfile = 'logfile.txt';
open(my $log, ">", $logfile);

system("pdftotext.exe", "input.pdf", "data.txt");

my $bank_summary = "data.txt";
open(my $data, '<', $bank_summary)
	or die "Could not open file '$bank_summary' $!";

my $temp1 = 'temp1.txt';
open(my $fh1, ">", $temp1)
	or die "Could not make file '$temp1' $!";

my $temp2 = 'temp2.txt';
open(my $fh2, ">", $temp2)
	or die "Could not make file '$temp2' $!";

while (my $line = <$data>) {
	chomp $line;
	if($line eq "Description") {
		# Get Transactions
		while (my $line = <$data>) {
			chomp $line;
			if($line =~ m/^Total.*$/) {
				last;
			}
			
			if(!($line eq "")) {
				$line =~ m/^(\d\d\/\d\d\/\d\d) (.*)$/;
				print $fh1 "$1,$2\n";
			}
		}
		
		# Get Amounts
		my $asdvasdf = 1;
		my $gettingAmounts = 0;
		while ($asdvasdf) {
			my $line = <$data>;
			chomp $line;
			if($line =~ m/^Amount.*$/) {
				$gettingAmounts = 1;
			}
			
			if ($line =~ m/\$/) {
				print $log "$line\n";
				$asdvasdf = 0;
				$line =~ s/-?\$.*$//;
			}
			
			if($gettingAmounts) {
				my @amounts = split " ", $line;
				foreach my $amount (@amounts) {
					if (!($amount eq "Amount")) {
						print $fh2 "$amount\n";
					}
				}
			}
		}
	}
}
##PAGE SKIPS
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
		if (!($line1 eq "")) {
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