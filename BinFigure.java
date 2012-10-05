
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Font;
import java.awt.Rectangle;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Iterator;
import java.util.List;
import uk.ac.starlink.xdoc.fig.Anchor;
import uk.ac.starlink.xdoc.fig.FigureIcon;
import uk.ac.starlink.xdoc.fig.StringDrawer;

/**
 * Generates the figures for BINARY and BINARY2 data layout.
 */
public class BinFigure extends FigureIcon {

    private static final int nd2 = 3;
    private static final int xCell = 14;
    private static final int yCell = 24;
    private static final int box = 18;
    private static final int fontSize = 16;
    private static final Color fixColor = Color.WHITE;
    private static final Color lenColor = new Color( 0xfdf44b );
    private static final Color varColor = new Color( 0xbfffff );
    private static final Color flagColor = new Color( 0xffb6fe );

    private final boolean isBinary2;
    private int xmax;
    private int ymax;

    public BinFigure( boolean isBinary2 ) {
        super( new Rectangle( 0, 0, 908,
                              160 + ( isBinary2 ? ( box + box / 2 ) : 0 ) ) );
        this.isBinary2 = isBinary2;
    }

    protected void doDrawing( Graphics2D g2 ) {
        g2.setColor( Color.BLACK );
        int gx = xCell / 4;
        int gy = yCell / 4;
        gy += drawRow( g2, gx, gy,
                       "Apple    ",
                       isBinary2 ? (short) 0 : (short) 99,
                       new int[] { 1, 2, 4, 8, 16 },
                       new float[] { 1.62f, 4.56f, 3.44f },
                       (byte) Integer.parseInt( "01000000", 2 ) );
        gy += drawRow( g2, gx, gy, 
                       "Orange   ",
                       (short) 15,
                       new int[] { 23, -11, 9 },
                       new float[] { 2.33f, 4.66f, 9.53f },
                       (byte) Integer.parseInt( "00000000", 2 ) );
        gy += drawKey( g2, gx, gy );
        ymax = gy;
    }

    private int drawRow( Graphics g, int gx, int gy,
                         String strVal, short shortVal,
                         int[] intsVal, float[] floatsVal, byte flags ) { 
        int ncol = 4;
        if ( isBinary2 ) {
            g.setFont( new Font( "Dialog", Font.PLAIN, fontSize ) );
            gx += drawCellFlags( g, gx, gy, flagColor, flags, ncol );
            gx += xCell;
        }
        xmax = Math.max( xmax, gx );

        g.setFont( new Font( "Monospaced", Font.BOLD, fontSize ) );
        for ( int i = 0; i < strVal.length(); i++ ) {
            gx += drawCell1( g, gx, gy, fixColor,
                             strVal.charAt( i ) );
        }

        g.setFont( new Font( "Dialog", Font.BOLD, fontSize ) );
        gx += xCell;
        gx += drawCellMulti( g, gx, gy, fixColor,
                             Short.toString( shortVal ), 2 );

        gx += xCell;
        gx += drawCellMulti( g, gx, gy, lenColor,
                             Integer.toString( intsVal.length ), 4 );
        for ( int i = 0; i < intsVal.length; i++ ) {
            gx += xCell;
            gx += drawCellMulti( g, gx, gy, varColor,
                                 Integer.toString( intsVal[ i ] ), 4 );
        }

        for ( int i = 0; i < floatsVal.length; i++ ) {
            gx += xCell;
            gx += drawCellMulti( g, gx, gy, fixColor,
                                 Float.toString( floatsVal[ i ] ), 4 );
        }

        return yCell + yCell / 2;
    }

    private int drawCell1( Graphics g, int gx, int gy, Color bg, char chr ) {
        paintRect( g, gx, gy, xCell, yCell, bg );
        new StringDrawer( Anchor.SOUTH, false, 1 )
           .drawString( g, "" + chr, gx + xCell / 2, gy + yCell - 2 );
        return xCell;
    }

    private int drawCellMulti( Graphics g, int gx, int gy, Color bg,
                               String txt, int nCell ) {
        paintRect( g, gx, gy, xCell * nCell, yCell, bg );
        for ( int i = 1; i < nCell; i++ ) {
            g.drawLine( gx + xCell * i, gy + yCell - yCell / 3,
                        gx + xCell * i, gy + yCell );
        }
        new StringDrawer( Anchor.SOUTH_EAST, false, 1 )
           .drawString( g, txt, gx + xCell * nCell - 2, gy + yCell - 2 );
        return xCell * nCell;
    }

    private int drawCellFlags( Graphics g, int gx, int gy, Color bg,
                               byte flags, int nSigFlags ) {
        String str = toBinaryString( flags );
        String str1 = str.substring( 0, nSigFlags );
        String str2 = str.substring( nSigFlags );
        int str1Width = getStringWidth( str1, g );
        int str2Width = getStringWidth( str2, g );
        int boxOff = str1Width + str2Width - xCell + 2;
        paintRect( g, gx + boxOff, gy, xCell, yCell, bg );
        StringDrawer stringer = new StringDrawer( Anchor.SOUTH_WEST, false, 1 );
        stringer.drawString( g, str1, gx, gy + yCell - 2 );
        Color color0 = g.getColor();
        g.setColor( Color.GRAY );
        stringer.drawString( g, str2, gx + str1Width, gy + yCell - 2 );
        g.setColor( color0 );
        return boxOff + xCell;
    }

    private static int getStringWidth( String str, Graphics g ) {
        return (int) Math.ceil( g.getFontMetrics().getStringBounds( str, g )
                                                  .getWidth() );
    }

    private String toBinaryString( byte flags ) {
        StringBuffer sbuf = new StringBuffer()
                           .append( Integer.toBinaryString( (int) flags ) );
        while ( sbuf.length() < 8 ) {
            sbuf.insert( 0, '0' );
        }
        return sbuf.toString();
    }

    private int drawKey( Graphics g, int gx, int gy ) {
        g.setFont( new Font( "SansSerif", Font.PLAIN, 18 ) );
        gx += 128;
        gy += drawKeyLine( g, gx, gy, fixColor, "Fixed length" );
        gy += drawKeyLine( g, gx, gy, lenColor,
                           "Length of variable length data" );
        gy += drawKeyLine( g, gx, gy, varColor, "Variable length data" );
        if ( isBinary2 ) {
            gy += drawKeyLine( g, gx, gy, flagColor, "Null flags (bits)" );
        }
        return gy;
    }

    private int drawKeyLine( Graphics g, int gx, int gy, Color bg,
                             String txt ) {
        int bw = box;
        int bh = box;
        paintRect( g, gx, gy, bw, bh, bg );
        new StringDrawer( Anchor.WEST, false, 1 )
           .drawString( g, txt, gx + bw + bw / 2, gy + bh / 2 );
        return bh + bh / 2;
    }

    private void paintRect( Graphics g, int gx, int gy, int width, int height,
                            Color bg ) {
        Color color0 = g.getColor();
        g.setColor( bg );
        g.fillRect( gx, gy, width, height );
        g.setColor( color0 );
        g.drawRect( gx, gy, width, height );
    }

    public static void main( String[] args ) throws IOException {
        String usage = "Usage: "
                     + BinFigure.class.getName()
                     + " binary|binary2";
        List<String> argList = new ArrayList<String>( Arrays.asList( args ) );
        String encoding = "";
        for ( Iterator<String> it = argList.iterator(); it.hasNext(); ) {
            String arg = it.next();
            if ( arg.equalsIgnoreCase( "BINARY" ) ||
                 arg.equalsIgnoreCase( "BINARY2" ) ) {
                it.remove();
                encoding = arg;
            }
        }
        final boolean isBinary2;
        if ( encoding.equalsIgnoreCase( "BINARY" ) ) {
            isBinary2 = false;
        }
        else if ( encoding.equalsIgnoreCase( "BINARY2" ) ) {
            isBinary2 = true;
        }
        else {
            System.err.println( usage );
            System.exit( 1 );
            return;
        }
        int status = runMain( argList.toArray( new String[ 0 ] ),
                              new BinFigure( isBinary2 ) );

        if ( status != 0 ) {
            System.exit( status );
        }
    }
}
