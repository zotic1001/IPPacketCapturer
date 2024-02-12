from enum import Enum

from scapy.all import IP, TCP, UDP, sniff, wrpcap
import csv


class SniffMode(Enum):
    COUNT = 1
    TIMEOUT = 2


def packet_capture(pcap_filename: str, sniff_mode: SniffMode, capture_value: int) -> None:
    if sniff_mode == SniffMode.COUNT:
        sniff(prn=lambda packet: wrpcap(pcap_filename, packet, append=True), count=capture_value)
    elif sniff_mode == SniffMode.TIMEOUT:
        sniff(prn=lambda packet: wrpcap(pcap_filename, packet, append=True), timeout=capture_value)
    print(f"Packets captured and saved to {pcap_filename}")


#  Read packets from pcap file and extract information
def process_packets(pcap_filename):
    packets_streams = {}

    def process_packet(packet):
        if IP in packet:
            ip_src = packet[IP].src
            ip_dst = packet[IP].dst

            if TCP in packet:
                src_port = packet[TCP].sport
                dst_port = packet[TCP].dport
            elif UDP in packet:
                src_port = packet[UDP].sport
                dst_port = packet[UDP].dport
            else:
                return
            stream_key = (ip_src, ip_dst, src_port, dst_port)
            if stream_key not in packets_streams:
                packets_streams[stream_key] = {'packets': 0, 'bytes': 0}
            packets_streams[stream_key]['packets'] += 1
            packets_streams[stream_key]['bytes'] += len(packet)

    sniff(offline=pcap_filename, prn=process_packet)
    return packets_streams


def write_to_csv(packets_streams, csv_file):
    with open(csv_file, 'w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(['Source IP', 'Destination IP', 'Source Port', 'Destination Port', 'Packets', 'Bytes'])

        for stream_key, stream_info in packets_streams.items():
            writer.writerow([stream_key[0], stream_key[1], stream_key[2], stream_key[3], stream_info['packets'],
                             stream_info['bytes']])


def main():
    while True:
        print("1 - to capture packets")
        print("2 - to create .cvs file from .pcap")
        print("3 - to close program")
        choose = int(input())
        if choose == 1:
            result_pcap_file = input("enter a name for the captured packet file (without .pcap): ") + ".pcap"
            print("1 - capture close by time-limit")
            print("2 - capture close by count of packets")
            mode = SniffMode(int(input()))
            value = int(input("input count of packets or timeout in seconds: "))
            packet_capture(result_pcap_file, mode, value)
        elif choose == 2:
            input_pcap_file = input("enter filename input .pcap file (with or without .pcap): ")
            if not input_pcap_file.endswith(".pcap"):
                input_pcap_file += ".pcap"
            output_csv_filename = input("enter output filename .cvs (with or without .cvs): ")
            if not output_csv_filename.endswith(".cvs"):
                output_csv_filename += ".cvs"
            streams = process_packets(input_pcap_file)
            write_to_csv(streams, output_csv_filename)
        elif choose == 3:
            return


if __name__ == '__main__':
    main()
