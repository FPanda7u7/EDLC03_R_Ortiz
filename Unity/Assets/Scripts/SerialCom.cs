using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System.IO.Ports;

public class SerialCom : MonoBehaviour
{
    //Arduino

    string[] ports;
    string portname;
    bool isConnected = false;
    SerialPort port;
    public Dropdown lista;

    //Canvas
    public Text estabilidad;
    public Text estado;
    public Text pause;
    public Text alerta;
    public Text perdiste;

    //Simulator
    public Image barrilA;
    public float contenidoA;
    public Image barrilB;
    public float contenidoB;
    public Image barrilC;
    public float contenidoC;

    public float contenidoMaximo = 100.0f;

    private void Awake()
    {
        lista.options.Clear();
        ports = SerialPort.GetPortNames();

        foreach (string port in ports)
        {
            lista.options.Add(new Dropdown.OptionData() { text = port });
        }

        DropdownItemSelected(lista);
        lista.onValueChanged.AddListener(delegate { DropdownItemSelected(lista); });
    }

    void Start()
    {
        pause.enabled = false;
        alerta.enabled = false;
        perdiste.enabled = false;

    }

    void Update()
    {
        if (isConnected)
        {
            string dataString = port.ReadLine();
            char splitChar = ';';
            string[] dataRAW = dataString.Split(splitChar);

            contenidoA = float.Parse(dataRAW[0]);
            contenidoB = float.Parse(dataRAW[1]);
            contenidoC = float.Parse(dataRAW[2]);


            barrilA.fillAmount = contenidoA / contenidoMaximo;
            barrilB.fillAmount = contenidoB / contenidoMaximo;
            barrilC.fillAmount = contenidoC / contenidoMaximo;

            estabilidad.text = "Estabilidad: " + dataRAW[3];
            estado.text = "Estado: " + dataRAW[4];

            if (dataRAW[5] == "1")
            {
                pause.enabled = true;
            }
            else
            {
                pause.enabled = false;
            }

            if (dataRAW[6] == "1" && dataRAW[7] == "0")
            {
                alerta.enabled = true;
            }
            else
            {
                alerta.enabled = false;
            }

            if (dataRAW[7] == "1")
            {
                perdiste.enabled = true;
            }
            else
            {
                perdiste.enabled = false;
            }
        }
    }

    void DropdownItemSelected(Dropdown lista)
    {
        int indice = lista.value;
        portname = lista.options[indice].text;
    }

    public void conectar()
    {
        if (!isConnected)
        {
            connect_to_Arduino();
        }       
    }

    public void desconectar()
    {
        if (isConnected)
        {
            disconnect_from_Arduino();
        }       
    }

    void connect_to_Arduino()
    {
        isConnected = true;
        port = new SerialPort(portname, 9600, Parity.None, 8, StopBits.One);

        port.Open();
        
    }

    void disconnect_from_Arduino()
    {
        isConnected = false;
        port.Close();
    }
}
