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
my $transactionsInQueue = 0;

while (my $outerline = <$data>) {
	chomp $outerline;
	
	# Get Transactions
	if($outerline eq "Description") {
		$tableCount = $tableCount + 1;
		my $gettingTransaction = 0;
		while (my $line = <$data>) {
			chomp $line;
			if($line =~ m/^Total.*$/) {
				print $fh1 "\n";
				$transactionsInQueue = $transactionsInQueue+1;
				last;
			}
			
			if($line =~ m/^Amount.*$/) {
				print $fh1 "\n";
				$transactionsInQueue = $transactionsInQueue+1;
				$outerline = $line;
				last;
			}
			
			
			if ($line =~ m/^(\d\d\/\d\d\/\d\d)(.*)$/) {
				if($gettingTransaction) {
					print $fh1 "\n";
					$transactionsInQueue = $transactionsInQueue+1;
				}
				my $date = $1;
				my $desc = $2;
				$desc =~ s/[",]//; #" chars would mess up the generated csv file
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
		my $end_early = 0;
		
		if ($outerline =~ m/\$/) {# check for $ on first line
			$outerline =~ s/-?\$.*$//;
			$end_early = 1;
		}
		
		my @amounts = split " ", $outerline;
		
		foreach my $amount (@amounts) {
			if (!($amount eq "Amount")) {
				$amount =~ s/[",]//;
				print $fh2 "$amount\n";
				$transactionsInQueue = $transactionsInQueue - 1;
			}
		}
		
		if ($end_early) {
			#do nothing
		} elsif ($transactionsInQueue == 0) {
			my $line;
			my $first = 1;
			while ($line = <$data>) {
				chomp $line;
				
				if($line eq "") {
					next;
				}
				
				if (!($line =~ m/^(\d\d\/\d\d\/\d\d)(.*)$/)) {
					$first = -1;
				}
				last;
			}
			
			while (1) {
				if ($first == 0) {
					$line = <$data>;
					chomp $line;
					if ($line eq "") {
						next;
					}
				} elsif ($first == 1) {
					$first = 0;
				} else {
					last;
				}
				
				if(!($line =~ m/^(\d\d\/\d\d\/\d\d)(.*)$/)){ #$line =~ m/^Total (.*)$/) {
					last;
				} else {
					my $date = $1;
					my $desc = $2;
					$desc =~ s/[",]//; #" chars would mess up the generated csv file
					print $fh1 "$date,$desc";
				}
				
				while(1){
					$line = <$data>;
					chomp $line;
					
					if ($line =~ m/^-?[\d,]*\.\d\d$/) {
						#$line =~ s/-?\$.*$//;
						$line =~ s/[",]//;
						print $fh2 "$line\n";
						print $fh1 "\n";
						last;
					} elsif (!($line eq "")) {
						$line =~ s/[",]//;
						print $fh1 " $line";
					}
				}
			}
		} else { # run through amounts like normal
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
					$amount =~ s/[",]//;
					print $fh2 "$amount\n";
					$transactionsInQueue = $transactionsInQueue - 1;
				}
			}
		}
	}
}

close($fh1);
close($fh2);
open($fh1, "<", $temp1);
open($fh2, "<", $temp2);

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
	$line1 =~ m/^(\d\d\/\d\d\/\d\d,) (.*)$/;
	print $fhout "\n$1$2,$line2"
}
