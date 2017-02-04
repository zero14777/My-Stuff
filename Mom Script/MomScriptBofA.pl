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
open(my $transactionsFH1, ">", $temp1)
	or die "Could not make file '$temp1' $!";

my $temp2 = 'temp2.txt';
open(my $transactionsFH2, ">", $temp2)
	or die "Could not make file '$temp2' $!";
	
my $temp3 = 'temp3.txt';
open(my $checksFH1, ">", $temp3)
	or die "Could not make file '$temp3' $!";

my $temp4 = 'temp4.txt';
open(my $checksFH2, ">", $temp4)
	or die "Could not make file '$temp4' $!";

my $transactionsInQueue = 0;
my @tables = ();

while (my $outerline = <$data>) {
	chomp $outerline;
	
	# Get Checks
	if($outerline eq "Check #") {
		push @tables, 'Checks';
		my $GettingAmounts = 0;
		while (my $line = <$data>) {
			chomp $line;
			
			if($line =~ m/^Amount.*$/ or $GettingAmounts) {
				if (!($line eq "")) {
					$GettingAmounts = 1;
					my @amounts = split " ", $line;
					foreach my $amount (@amounts) {
						if (!($amount eq "Amount")) {
							$amount =~ s/[",]//;
							print $checksFH2 "$amount\n";
						}
					}
				} else {
					$GettingAmounts = 0;
				}
			}
			if($line =~ m/^Total.*$/) {
				last;
			}
			if ($line =~ m/^(\d\d\/\d\d\/\d\d) (.*)?$/) {
				my $date = $1;
				my $desc = $2;
				$desc =~ s/[",\*]//;
				print $checksFH1 "$date, Check # $desc\n";
			}
		}
	}
	
	# Get Transactions
	if($outerline eq "Description" or $outerline eq "Transaction description") {
		push @tables, 'Transactions';
		my $gettingTransaction = 0;
		while (my $line = <$data>) {
			chomp $line;
			if($line =~ m/^Total.*$/) {
				print $transactionsFH1 "\n";
				$transactionsInQueue = $transactionsInQueue+1;
				last;
			}
			
			if($line =~ m/^Amount.*$/) {
				print $transactionsFH1 "\n";
				$transactionsInQueue = $transactionsInQueue+1;
				$outerline = $line;
				last;
			}
			
			
			if ($line =~ m/^(\d\d\/\d\d\/\d\d)(.*)$/) {
				if($gettingTransaction) {
					print $transactionsFH1 "\n";
					$transactionsInQueue = $transactionsInQueue+1;
				}
				my $date = $1;
				my $desc = $2;
				$desc =~ s/[",]//; #" chars would mess up the generated csv file
				print $transactionsFH1 "$date,$desc";
				$gettingTransaction = 1;
			} elsif (!($line eq "") and $gettingTransaction) {
				$line =~ s/[",]//;
				print $transactionsFH1 " $line";
			}
		}
	}
	
	# Get Amounts
	if($outerline =~ m/^Amount.*$/ and @tables) {# Be aware of picking up too many amounts
		my $AmountType = shift @tables;
		my $fh2;
		if ($AmountType eq "Checks") {
			$fh2 = $checksFH2;
		} elsif ($AmountType eq "Transactions") {
			$fh2 = $transactionsFH2;
		}
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
		} elsif ($transactionsInQueue == 0 and $AmountType eq "Transactions") { # should automatically catch end continued on next page
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
					print $transactionsFH1 "$date,$desc";
				}
				
				while(1){
					$line = <$data>;
					chomp $line;
					
					if ($line =~ m/^-?[\d,]*\.\d\d$/) {
						#$line =~ s/-?\$.*$//;
						$line =~ s/[",]//;
						print $transactionsFH2 "$line\n";
						print $transactionsFH1 "\n";
						last;
					} elsif (!($line eq "")) {
						$line =~ s/[",]//;
						print $transactionsFH1 " $line";
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

close($transactionsFH1);
close($transactionsFH2);
open($transactionsFH1, "<", $temp1);
open($transactionsFH2, "<", $temp2);

my $output = 'output.csv';
open(my $fhout, ">", $output);

print $fhout "Date,Description,Amount";

while (1) {
	my $line1 = <$transactionsFH1>;
	chomp $line1;
	my $line2 = <$transactionsFH2>;
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

close($checksFH1);
close($checksFH2);
open($checksFH1, "<", $temp3);
open($checksFH2, "<", $temp4);

while (1) {
	my $line1 = <$checksFH1>;
	chomp $line1;
	my $line2 = <$checksFH2>;
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