// DISCLAIMER - THIS PROGRAM IS HIGHLY UNOPTIMIZED. USE AT YOUR OWN RISK.
// DISCLAIMER - THIS PROGRAM WAS ONLY CREATED FOR THE FUN OF IT. IT IS NOT TO BE USED FOR THE MINECRAFT VIDEOS.
// DISCLAIMER - ALL OTHER TRANSCRIPTIONS WILL BE/HAVE BEEN DONE BY HAND UNLESS STATED OTHERWISE IN A VIDEO.
// DISCLAIMER - I STILL HAVE THE SHEET MUSIC FROM THE NOTATION EDITOR THAT CORRESPONDS TO THE MUSIC IN THE VIDEOS.
// DISCLAIMER - YOU CAN ALSO VIEW SOME SAMPLE MIDIS IN THE OTHER REPO AND YOU WILL SEE THAT IT IS TOTALLY DIFFERENT FROM WHAT THIS OUTPUTS.

// Java program showing the implementation of a simple record
import javax.sound.midi.*;
import java.io.File;
import java.io.IOException;
import java.util.*;

public class MIDISEPARATOR {
    public static final int NOTE_ON = 0x90;
    public static final int NOTE_OFF = 0x80;
    public static final String[] NOTE_NAMES = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};


    // Algorithmic variables

    static TreeMap<Long, ArrayList<Integer>> keysByTime;
    static void push(Long time, Integer key) {
        if (!keysByTime.containsKey(time)) keysByTime.put(time, new ArrayList<Integer>());
        keysByTime.get(time).add(key);
    }

    // Arranges music into a MIDI format that is directly portable to OpenNBS.

    public static void main(String[] args) throws InvalidMidiDataException, IOException {
        while (true) {
            // Initialize variables

            keysByTime = new TreeMap();

            // Begin

            Scanner sc = new Scanner(System.in);
            String name = sc.nextLine();
            if (name.equals("")) return;

            Sequence sequence = new Sequence(30, 30);

            try {
                sequence = MidiSystem.getSequence(new File(name + ".mid"));
            }
            catch (Exception e) {
                System.out.println("ERROR 404");
                continue;
            }

            int trackNumber = 0;
            for (Track track : sequence.getTracks()) {
                trackNumber++;
                System.out.println("Track " + trackNumber + ": size = " + track.size());
                System.out.println();
                for (int i = 0; i < track.size(); i++) {
                    MidiEvent event = track.get(i);
                    System.out.print("@" + event.getTick() + " ");
                    MidiMessage message = event.getMessage();
                    if (message instanceof ShortMessage) {
                        ShortMessage sm = (ShortMessage) message;
                        System.out.print("Channel: " + sm.getChannel() + " | ");
                        if (sm.getCommand() == NOTE_ON) {
                            int key = sm.getData1();
                            int octave = (key / 12) - 1;
                            int note = key % 12;
                            String noteName = NOTE_NAMES[note];
                            int velocity = sm.getData2();
                            System.out.println("Note on, " + noteName + octave + " key=" + key + " velocity: " + velocity);

                            push(event.getTick(), key);
                        } else if (sm.getCommand() == NOTE_OFF) {
                            int key = sm.getData1();
                            int octave = (key / 12) - 1;
                            int note = key % 12;
                            String noteName = NOTE_NAMES[note];
                            int velocity = sm.getData2();
                            System.out.println("Note off, " + noteName + octave + " key=" + key + " velocity: " + velocity);
                        } else {
                            System.out.println("Command:" + sm.getCommand());
                        }
                    } else {
                        System.out.println("Other message: " + message.getClass());
                    }
                }
            }

            // tetSystem.out.println(keysByTime);

            // Double bass = -INF - 3, Harp = 3 - 5, Bell = 5 - INF. All intervals are closed on the left (except -INF) and open on the right.
            int maximumchannels[] = new int[3];
            int currentchannels[] = new int[3];

            for (Long i : keysByTime.keySet()) {
                for (int in = 0; in < 3; in++) currentchannels[in] = 0;

                ArrayList<Integer> vals = keysByTime.get(i);
                Collections.sort(vals);
                for (int x : vals) {
                    if (x < 54) currentchannels[0]++;
                    else if (x < 54 + 24) currentchannels[1]++;
                    else currentchannels[2]++;
                }
                for (int in = 0; in < 3; in++) maximumchannels[in] = Math.max(maximumchannels[in], currentchannels[in]);
            }

            System.out.println(Arrays.toString(maximumchannels));

            // Create the separated midi

            Sequence res = new Sequence(sequence.getDivisionType(), sequence.getResolution());

            for (int i : maximumchannels) {
                for (int x = 0; x < i; x++) res.createTrack();
            }

            System.out.println("" + res.getTracks().length + " TRACKS IN FINAL EXPORT...");

            for (Long integer : keysByTime.keySet()) {
                ArrayList<Integer> vals = keysByTime.get(integer);
                Collections.sort(vals);
                int index = 0;
                int status = 0;
                for (int i : vals) {
                    if (i < 54) status = 0;
                    else if (i < 54 + 24) status = 1;
                    else status = 2;
                    ShortMessage ON = new ShortMessage(NOTE_ON + status, i, 64);
                    ShortMessage OFF = new ShortMessage(NOTE_OFF + status, i, 64);

                    res.getTracks()[index].add(new MidiEvent(ON, integer));
                    res.getTracks()[index].add(new MidiEvent(OFF, integer + res.getResolution() - 1));
                    index++;
                }
            }

            File file = new File(name + "_output.mid");

            MidiSystem.write(res, MidiSystem.getMidiFileTypes(sequence)[0], file);

        }
    }
}
