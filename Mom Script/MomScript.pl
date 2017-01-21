#!/usr/bin/perl

use warnings;
use strict;
#use OLE;
#use Win32::OLE::Const "Microsoft Excel";
use Data::Dumper qw(Dumper);

  
system("pdftotext.exe", "input.pdf", "data.txt");

my $bank_summary = "data.txt";
open(my $data, '<:encoding(UTF-8)', $bank_summary)
	or die "Could not open file '$bank_summary' $!";

my $output = 'output.txt';
open(my $fh2, ">", $output);

#my $excel = CreateObject OLE "Excel.Application";
#my $workbook = $excel -> Workbooks -> Add;
#my $sheet = $workbook -> Worksheets("Summary");
#$sheet -> Activate;

#use Excel::Writer::XLSX;
#my $workbook  = Excel::Writer::XLSX->new( 'simple.xlsx' );
#my $worksheet = $workbook->add_worksheet();
#$worksheet->write( "A1", "Hi Excel!" );
#$worksheet->write( "A2", "second row" );
#$workbook->close;

#use Spreadsheet::WriteExcel;
# Create a new Excel workbook
#my $workbook = Spreadsheet::WriteExcel->new('perl.xls');
# Add a worksheet
#$worksheet = $workbook->add_worksheet();
#$worksheet->write(0, 0,  'Hi Excel!');
#$workbook->close();

while (my $line = <$data>) {
	chomp $line;
	if($line eq "Description") {
		while (my $line = <$data>) {
			chomp $line;
			if($line =~ m/^Total.*$/) {
				last;
			}
			
			if(!($line eq "")) {
				print $fh2 "$line\n";
			}
		}
		
		while (my $line = <$data>) {
			chomp $line;
			if($line =~ m/^Amount.*$/) {
				my @amounts = split " ", $line;
				foreach my $amount (@amounts) {
					print $fh2 "$amount\n"; #NOT PRINTING ALL AMOUNTS WIERD FORMAT
				}
				last;
			}
		}
	}
}

sub hmmm {
	my $data = shift(@_);
    }