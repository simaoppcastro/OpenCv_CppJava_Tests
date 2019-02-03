package POO_projecto;


import POO_projecto.measureSystem;
import java.sql.*;
import java.awt.Image;
import java.awt.Toolkit;
import javax.swing.*;

//librarias opencv/javacv
import com.googlecode.javacpp.Loader;
import com.googlecode.javacv.*;
import com.googlecode.javacv.cpp.*;
import com.googlecode.javacv.cpp.opencv_core.CvPoint;
import com.googlecode.javacv.cpp.opencv_core.CvScalar;
import com.googlecode.javacv.cpp.opencv_core.CvSeq;
import com.googlecode.javacv.cpp.opencv_core.IplImage;
import com.googlecode.javacv.cpp.opencv_highgui.CvCapture;
import com.googlecode.javacv.cpp.opencv_imgproc.CvMoments;
import static com.googlecode.javacv.cpp.opencv_core.*;
import static com.googlecode.javacv.cpp.opencv_imgproc.*;
import static com.googlecode.javacv.cpp.opencv_calib3d.*;
import static com.googlecode.javacv.cpp.opencv_objdetect.*;
import static com.googlecode.javacv.cpp.opencv_highgui.*;

/**
 * @author simaoppcastro
 */
public class VisualApp_measureSystem extends javax.swing.JFrame //implements main_measureSystem
{
    
    //------------------------------------------------------------
    //conexao a base de dados
    // JDBC driver name and database URL
    String JDBC_DRIVER = "com.mysql.jdbc.Driver";  //jar file é um driver para utilizar o mysql
    String DB_URL = "jdbc:mysql://localhost/medidas";

    //  Database credentials
    String USER = "root";
    String PASS = "";

    //public static void main(String[] args) {
    Connection conn = null;
    Statement stmt = null;
    //fim dao codigo da conexao a base de dados
    //--------------------------------------------------------------

    public VisualApp_measureSystem() 
    {
        initComponents();
    }

    //CODIGO GERADO AUTOMATICAMENTE 
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        campoTela1 = new javax.swing.JTextField();
        campoTela2 = new javax.swing.JTextField();
        campoReforco = new javax.swing.JTextField();
        campoParedes = new javax.swing.JTextField();
        jLabel2 = new javax.swing.JLabel();
        jLabel3 = new javax.swing.JLabel();
        jLabel4 = new javax.swing.JLabel();
        jLabel5 = new javax.swing.JLabel();
        jLabel6 = new javax.swing.JLabel();
        jLabel7 = new javax.swing.JLabel();
        BotaoPesquisaReceita = new javax.swing.JButton();
        jLabel8 = new javax.swing.JLabel();
        jLabel9 = new javax.swing.JLabel();
        ligarBaseDados = new javax.swing.JButton();
        camadaLida = new javax.swing.JTextField();
        tela1Lida = new javax.swing.JTextField();
        tela2Lida = new javax.swing.JTextField();
        reforcoLido = new javax.swing.JTextField();
        paredeLido = new javax.swing.JTextField();
        jLabel1 = new javax.swing.JLabel();
        jLabel10 = new javax.swing.JLabel();
        campoID = new javax.swing.JTextField();
        jLabel11 = new javax.swing.JLabel();
        campoCamada = new javax.swing.JTextField();
        campoPesquisaReceita = new javax.swing.JTextField();
        jLabel12 = new javax.swing.JLabel();
        accuCamada = new javax.swing.JTextField();
        accuTela1 = new javax.swing.JTextField();
        accuTela2 = new javax.swing.JTextField();
        accuReforco = new javax.swing.JTextField();
        accuPerede = new javax.swing.JTextField();
        jLabel14 = new javax.swing.JLabel();
        ligarCamera = new javax.swing.JButton();
        labelCamera = new javax.swing.JLabel();
        jLabel15 = new javax.swing.JLabel();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setTitle("Vision_Machine");
        setBackground(new java.awt.Color(255, 255, 255));
        setCursor(new java.awt.Cursor(java.awt.Cursor.DEFAULT_CURSOR));
        setIconImage(getIconImage());

        campoTela1.setBackground(new java.awt.Color(255, 204, 204));

        campoTela2.setBackground(new java.awt.Color(255, 204, 204));

        campoReforco.setBackground(new java.awt.Color(255, 204, 204));

        campoParedes.setBackground(new java.awt.Color(255, 204, 204));

        jLabel2.setForeground(new java.awt.Color(51, 0, 102));
        jLabel2.setText("ID_Receita:");

        jLabel3.setForeground(new java.awt.Color(51, 0, 102));
        jLabel3.setText("Largura Camada:");

        jLabel4.setForeground(new java.awt.Color(51, 0, 102));
        jLabel4.setText("Largura Tela1:");

        jLabel5.setForeground(new java.awt.Color(51, 0, 102));
        jLabel5.setText("Largura Tela2:");

        jLabel6.setForeground(new java.awt.Color(51, 0, 102));
        jLabel6.setText("Largura Reforço:");

        jLabel7.setForeground(new java.awt.Color(51, 0, 102));
        jLabel7.setText("Largura Paredes:");

        BotaoPesquisaReceita.setText("SubmeterReceita!");
        BotaoPesquisaReceita.setBorder(new javax.swing.border.SoftBevelBorder(javax.swing.border.BevelBorder.RAISED));
        BotaoPesquisaReceita.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                BotaoPesquisaReceitaActionPerformed(evt);
            }
        });

        jLabel8.setForeground(new java.awt.Color(51, 0, 102));
        jLabel8.setText("Inserir Receita:");

        jLabel9.setForeground(new java.awt.Color(102, 102, 102));
        jLabel9.setText("@simaoppcastro");

        ligarBaseDados.setForeground(new java.awt.Color(255, 51, 51));
        ligarBaseDados.setText("LigarBaseDados!");
        ligarBaseDados.setBorder(new javax.swing.border.SoftBevelBorder(javax.swing.border.BevelBorder.RAISED));
        ligarBaseDados.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                ligarBaseDadosActionPerformed(evt);
            }
        });

        camadaLida.setBackground(new java.awt.Color(255, 204, 204));

        tela1Lida.setBackground(new java.awt.Color(255, 204, 204));

        tela2Lida.setBackground(new java.awt.Color(255, 204, 204));

        reforcoLido.setBackground(new java.awt.Color(255, 204, 204));

        paredeLido.setBackground(new java.awt.Color(255, 204, 204));

        jLabel1.setForeground(new java.awt.Color(51, 0, 102));
        jLabel1.setText("Vision_Machine");

        jLabel10.setForeground(new java.awt.Color(51, 0, 102));
        jLabel10.setText("Valores de Receita");

        campoID.setBackground(new java.awt.Color(255, 204, 204));

        jLabel11.setForeground(new java.awt.Color(51, 0, 102));
        jLabel11.setText("ValoresMedidos");

        campoCamada.setBackground(new java.awt.Color(255, 204, 204));

        campoPesquisaReceita.setBackground(new java.awt.Color(255, 204, 204));

        jLabel12.setForeground(new java.awt.Color(51, 0, 102));
        jLabel12.setText("Accuracy (%)");

        accuCamada.setBackground(new java.awt.Color(255, 204, 204));

        accuTela1.setBackground(new java.awt.Color(255, 204, 204));

        accuTela2.setBackground(new java.awt.Color(255, 204, 204));

        accuReforco.setBackground(new java.awt.Color(255, 204, 204));

        accuPerede.setBackground(new java.awt.Color(255, 204, 204));

        jLabel14.setIcon(new javax.swing.ImageIcon(getClass().getResource("/POO_projecto/visionapp.png"))); // NOI18N

        ligarCamera.setForeground(new java.awt.Color(255, 0, 51));
        ligarCamera.setText("Ligar!");
        ligarCamera.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                ligarCameraActionPerformed(evt);
            }
        });

        labelCamera.setForeground(new java.awt.Color(51, 0, 102));
        labelCamera.setText("Ligação Sistema Visão");

        jLabel15.setIcon(new javax.swing.ImageIcon(getClass().getResource("/POO_projecto/database.png"))); // NOI18N

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addContainerGap()
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                                        .addComponent(tela1Lida, javax.swing.GroupLayout.PREFERRED_SIZE, 90, javax.swing.GroupLayout.PREFERRED_SIZE)
                                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                                        .addComponent(accuTela1, javax.swing.GroupLayout.PREFERRED_SIZE, 32, javax.swing.GroupLayout.PREFERRED_SIZE)
                                        .addGap(24, 24, 24))
                                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                                        .addComponent(camadaLida, javax.swing.GroupLayout.PREFERRED_SIZE, 90, javax.swing.GroupLayout.PREFERRED_SIZE)
                                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                                        .addComponent(accuCamada, javax.swing.GroupLayout.PREFERRED_SIZE, 32, javax.swing.GroupLayout.PREFERRED_SIZE)
                                        .addGap(21, 21, 21))
                                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                                        .addComponent(tela2Lida, javax.swing.GroupLayout.PREFERRED_SIZE, 90, javax.swing.GroupLayout.PREFERRED_SIZE)
                                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                                        .addComponent(accuTela2, javax.swing.GroupLayout.PREFERRED_SIZE, 32, javax.swing.GroupLayout.PREFERRED_SIZE)
                                        .addGap(24, 24, 24))
                                    .addGroup(layout.createSequentialGroup()
                                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                            .addGroup(layout.createSequentialGroup()
                                                .addComponent(reforcoLido, javax.swing.GroupLayout.PREFERRED_SIZE, 90, javax.swing.GroupLayout.PREFERRED_SIZE)
                                                .addGap(60, 60, 60)
                                                .addComponent(accuReforco, javax.swing.GroupLayout.PREFERRED_SIZE, 32, javax.swing.GroupLayout.PREFERRED_SIZE))
                                            .addGroup(layout.createSequentialGroup()
                                                .addComponent(paredeLido, javax.swing.GroupLayout.PREFERRED_SIZE, 90, javax.swing.GroupLayout.PREFERRED_SIZE)
                                                .addGap(58, 58, 58)
                                                .addComponent(accuPerede, javax.swing.GroupLayout.PREFERRED_SIZE, 32, javax.swing.GroupLayout.PREFERRED_SIZE)))
                                        .addGap(0, 0, Short.MAX_VALUE)))
                                .addGap(41, 41, 41))
                            .addGroup(layout.createSequentialGroup()
                                .addComponent(jLabel11)
                                .addGap(47, 47, 47)
                                .addComponent(jLabel12)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 37, Short.MAX_VALUE)))
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(campoTela2, javax.swing.GroupLayout.PREFERRED_SIZE, 99, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                                .addComponent(campoReforco, javax.swing.GroupLayout.PREFERRED_SIZE, 99, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addComponent(campoParedes, javax.swing.GroupLayout.PREFERRED_SIZE, 99, javax.swing.GroupLayout.PREFERRED_SIZE))
                            .addComponent(jLabel10)
                            .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING, false)
                                .addComponent(campoCamada, javax.swing.GroupLayout.Alignment.LEADING, javax.swing.GroupLayout.DEFAULT_SIZE, 99, Short.MAX_VALUE)
                                .addComponent(campoTela1, javax.swing.GroupLayout.Alignment.LEADING))))
                    .addGroup(layout.createSequentialGroup()
                        .addGap(94, 94, 94)
                        .addComponent(ligarBaseDados, javax.swing.GroupLayout.PREFERRED_SIZE, 170, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jLabel15)))
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                .addGap(0, 0, Short.MAX_VALUE)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jLabel9, javax.swing.GroupLayout.Alignment.TRAILING)
                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                        .addComponent(jLabel8, javax.swing.GroupLayout.PREFERRED_SIZE, 86, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(6, 6, 6)
                        .addComponent(campoPesquisaReceita, javax.swing.GroupLayout.PREFERRED_SIZE, 54, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(BotaoPesquisaReceita)
                        .addGap(41, 41, 41))
                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(labelCamera)
                            .addGroup(layout.createSequentialGroup()
                                .addComponent(jLabel2)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(campoID, javax.swing.GroupLayout.PREFERRED_SIZE, 62, javax.swing.GroupLayout.PREFERRED_SIZE)))
                        .addGap(110, 110, 110))
                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                        .addComponent(jLabel14, javax.swing.GroupLayout.PREFERRED_SIZE, 59, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(158, 158, 158))
                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                        .addComponent(jLabel1, javax.swing.GroupLayout.PREFERRED_SIZE, 92, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(140, 140, 140))
                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(jLabel4)
                            .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                                .addComponent(jLabel3, javax.swing.GroupLayout.PREFERRED_SIZE, 99, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addComponent(ligarCamera))
                            .addComponent(jLabel5)
                            .addComponent(jLabel6, javax.swing.GroupLayout.PREFERRED_SIZE, 99, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(jLabel7))
                        .addGap(147, 147, 147))))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addComponent(jLabel1, javax.swing.GroupLayout.PREFERRED_SIZE, 16, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(8, 8, 8)
                .addComponent(jLabel14)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(ligarBaseDados)
                    .addComponent(jLabel15))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(BotaoPesquisaReceita)
                    .addComponent(campoPesquisaReceita, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jLabel8))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(campoID, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jLabel2))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(labelCamera)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(ligarCamera)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 17, Short.MAX_VALUE)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel12)
                    .addComponent(jLabel10)
                    .addComponent(jLabel11, javax.swing.GroupLayout.PREFERRED_SIZE, 16, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(9, 9, 9)
                .addComponent(jLabel3)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(campoCamada, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(accuCamada, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(camadaLida, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addGap(9, 9, 9)
                        .addComponent(jLabel4)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(accuTela1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(tela1Lida, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addGap(4, 4, 4)
                        .addComponent(jLabel5)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(accuTela2, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(tela2Lida, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
                    .addGroup(layout.createSequentialGroup()
                        .addGap(40, 40, 40)
                        .addComponent(campoTela1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(30, 30, 30)
                        .addComponent(campoTela2, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addComponent(jLabel6)
                .addGap(14, 14, 14)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(campoReforco, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(accuReforco, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(reforcoLido, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jLabel7)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(campoParedes, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(accuPerede, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(paredeLido, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(41, 41, 41)
                .addComponent(jLabel9)
                .addGap(18, 18, 18))
        );

        labelCamera.getAccessibleContext().setAccessibleName("Ligação ao Sistema de Visão");

        pack();
    }// </editor-fold>//GEN-END:initComponents

    //BASE DADOS
    private void BotaoPesquisaReceitaActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_BotaoPesquisaReceitaActionPerformed
        //estas duas primeiras linha de codigo logo abaixo foi para testar apenas
        //String aux = jTextField1.getText();  //vai buscar o texto que existe para pesquisa
        //jTextArea1.append(aux + "\n");     //depois cola o texto na caixa   (o \n é para mudar de linha)

        //****codigo abaixo para a conexao a base de dados****
        try {
            //STEP 4: Execute a query
            System.out.println("Creating statement...");
            stmt = conn.createStatement();
            String sql;
            String idMedida = campoPesquisaReceita.getText();           //criterio é a variavel pela qual chamamos quando pesquisamos por exemplo por um nome dos contactos
            sql = "SELECT* FROM receitas WHERE id = '" + idMedida + "'";
            ResultSet rs = stmt.executeQuery(sql);
            //stmt.executeUpdate(sql, columnNames); este stmt statement permite fazer os updates, delete na base de dados

            //STEP 5: Extract data from result set
            while (rs.next()) {
                //Retrieve by column name
                int id = rs.getInt("id");
                double l_camada = rs.getDouble("l_camada");
                double l_tela1 = rs.getDouble("l_tela1");
                double l_tela2 = rs.getDouble("l_tela2");
                double l_tira = rs.getDouble("l_tira");
                double l_parede = rs.getDouble("l_parede");

                //Display values
                campoID.setText(Integer.toString(id));
                campoCamada.setText(Double.toString(l_camada) + " mm");
                campoTela1.setText(Double.toString(l_tela1) + " mm");
                campoTela2.setText(Double.toString(l_tela2) + " mm");
                campoReforco.setText(Double.toString(l_tira) + " mm");
                campoParedes.setText(Double.toString(l_parede) + " mm");

            }
            //STEP 6: Clean-up environment
            rs.close();
            //stmt.close();
            //conn.close();
        } catch (SQLException se) {
            //Handle errors for JDBC
            se.printStackTrace();
        } catch (Exception e) {
            //Handle errors for Class.forName
            e.printStackTrace();
        }

        System.out.println("Ligação/Pesquisa bem sucedida!");

    }//GEN-LAST:event_BotaoPesquisaReceitaActionPerformed
    //DRIVER PARA A LIGAÇÃO DA BASE DE DADOS
    private void ligarBaseDadosActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_ligarBaseDadosActionPerformed
        // TODO add your handling code here:
        try {
            //STEP 2: Register JDBC driver
            Class.forName("com.mysql.jdbc.Driver");

            //STEP 3: Open a connection
            System.out.println("Connecting to database...");
            conn = DriverManager.getConnection(DB_URL, USER, PASS);
        } catch (Exception e) {

        }
    }//GEN-LAST:event_ligarBaseDadosActionPerformed

    private void ligarCameraActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_ligarCameraActionPerformed
       
        measureSystem x = new measureSystem();
        
    }//GEN-LAST:event_ligarCameraActionPerformed
    //MAIN
    public static void main(String args[]) 
    {
        java.awt.EventQueue.invokeLater(new Runnable() 
        {
            public void run() 
            {
                new VisualApp_measureSystem().setVisible(true);
            }
        }
        );
        
        //main_measureSystem.main(args);     //para iniciar class do projecto opencv
    }

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton BotaoPesquisaReceita;
    private javax.swing.JTextField accuCamada;
    private javax.swing.JTextField accuPerede;
    private javax.swing.JTextField accuReforco;
    private javax.swing.JTextField accuTela1;
    private javax.swing.JTextField accuTela2;
    private javax.swing.JTextField camadaLida;
    private javax.swing.JTextField campoCamada;
    private javax.swing.JTextField campoID;
    private javax.swing.JTextField campoParedes;
    private javax.swing.JTextField campoPesquisaReceita;
    private javax.swing.JTextField campoReforco;
    private javax.swing.JTextField campoTela1;
    private javax.swing.JTextField campoTela2;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JLabel jLabel10;
    private javax.swing.JLabel jLabel11;
    private javax.swing.JLabel jLabel12;
    private javax.swing.JLabel jLabel14;
    private javax.swing.JLabel jLabel15;
    private javax.swing.JLabel jLabel2;
    private javax.swing.JLabel jLabel3;
    private javax.swing.JLabel jLabel4;
    private javax.swing.JLabel jLabel5;
    private javax.swing.JLabel jLabel6;
    private javax.swing.JLabel jLabel7;
    private javax.swing.JLabel jLabel8;
    private javax.swing.JLabel jLabel9;
    private javax.swing.JLabel labelCamera;
    private javax.swing.JButton ligarBaseDados;
    private javax.swing.JButton ligarCamera;
    private javax.swing.JTextField paredeLido;
    private javax.swing.JTextField reforcoLido;
    private javax.swing.JTextField tela1Lida;
    private javax.swing.JTextField tela2Lida;
    // End of variables declaration//GEN-END:variables
}
