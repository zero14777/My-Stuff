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

my $tableCount = 0;

while (my $outerline = <$data>) {
	chomp $outerline;
	
	# Get Transactions
	if($outerline eq "Description") {
		$tableCount = $tableCount + 1;
		my $gettingTransaction = 0;
		while (my $line = <$data>) {
			chomp $line;
			if($line =~ m/^Total.*$/) {
				print $fh1 "\"\n";
				last;
			}
			
			if($line =~ m/^Amount.*$/) {
				print $fh1 "\"\n";
				$outerline = $line;
				last;
			}
			
			
			if ($line =~ m/^(\d\d\/\d\d\/\d\d)(.*)$/) {
				if($gettingTransaction) {
					print $fh1 "\"\n";
				}
				my $date = $1;
				$date = "\"$date\"";
				my $desc = $2;
				$desc =~ s/"//; #" chars would mess up the generated csv file
				$desc = "\"$desc";
				print $fh1 "$date,$desc";
				$gettingTransaction = 1;
			} elsif (!($line eq "") and $gettingTransaction) {
				print $fh1 " $line";
			}
		}
	}
	
	# Get Amounts
	if($outerline =~ m/^Amount.*$/ and $tableCount > 0) {# Be aware of picking up too many amounts
		$tableCount = $tableCount - 1;
		my @amounts = split " ", $outerline;
		foreach my $amount (@amounts) {
			if (!($amount eq "Amount")) {
				$amount = "\"$amount\"";
				print $fh2 "$amount\n";
			}
		}
		my $gettingAmounts = 1;
		while ($gettingAmounts) {
			my $line = <$data>;
			chomp $line;
			
			if($line eq "continued on the next page") {
				last;
			}
			
			if ($line =~ m/\$/) {
				$gettingAmounts = 0;
				$line =~ s/-?\$.*$//;
			}
			
			my @amounts = split " ", $line;
			foreach my $amount (@amounts) {
				$amount = "\"$amount\"";
				print $fh2 "$amount\n";
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