while (1)

    {

        if (love >= 520)

        {

            for (int j = 0; j != 10; j++)

            {

                pthread_cancel(pth[j]);//杀死线程

                pthread_join(pth[j], NULL);//回收线程

            }

            break;

        }

    }
